"""
读取TFRECORD格式文件进行训练，将训练结果保存到model文件夹中，训练过程保存到logs文件夹中
"""
import tensorflow as tf
from model import Model
from model import width_input, hight_input, depth_input
import conf

# max_steps = 330240
max_steps = 20001


def parse_data(example_proto):
    features = tf.parse_single_example(example_proto,
                                       features={
                                           'image': tf.FixedLenFeature([], tf.string),
                                           'label': tf.FixedLenFeature([], tf.int64),
                                       })
    image = tf.decode_raw(features['image'], tf.uint8)
    image = tf.reshape(image, [width_input, hight_input, depth_input])
    image = tf.cast(image, tf.float32)/255.0
    label = tf.cast(features['label'], tf.int64)
    return image, label


def load_dataset(sess):
    # 加载训练的图片的信息和标签
    dataset_train = tf.data.TFRecordDataset(conf.TFRECORD_TRAIN_PATH)
    dataset_train = dataset_train.map(parse_data).repeat().shuffle(50000).batch(256)
    iterator_train = dataset_train.make_initializable_iterator()
    image_batch_train, label_batch_train = iterator_train.get_next()
    sess.run(iterator_train.initializer)
    # 加载验证的图片的信息和标签
    dataset_valid = tf.data.TFRecordDataset(conf.TFRECORD_VALID_PATH)
    dataset_valid = dataset_valid.map(parse_data).repeat().shuffle(50000).batch(512)
    iterator_valid = dataset_valid.make_initializable_iterator()
    image_batch_valid, label_batch_valid = iterator_valid.get_next()
    sess.run(iterator_valid.initializer)
    return image_batch_train, label_batch_train, image_batch_valid, label_batch_valid


def train():
    """
    训练函数
    :return: None
    """
    model = Model()
    sess = tf.Session()

    # 保存训练过程图
    writer_train = tf.summary.FileWriter('logs/train/', sess.graph)
    writer_valid = tf.summary.FileWriter('logs/valid/', sess.graph)
    image_batch_train, label_batch_train, image_batch_valid, label_batch_valid = load_dataset(sess)
    # 初始化所有变量
    sess.run(tf.global_variables_initializer())
    # 保存
    saver = tf.train.Saver()
    # 多线程
    coord = tf.train.Coordinator()
    threads = tf.train.start_queue_runners(sess=sess, coord=coord)

    # 读取以及学习好的模型，并接下去学习
    # saver.restore(sess, conf.MODEL_PATH)
    # saver = tf.train.Saver(max_to_keep=10)

    for i in range(max_steps):
        # 获取一个batch的训练图片和标签信息
        batch_x_train, batch_y_train = sess.run([image_batch_train, label_batch_train])

        # 传入模型进行训练
        _, loss_value = sess.run([model.optimizer, model.loss], feed_dict={model.X: batch_x_train,
                                                                           model.Y: batch_y_train,
                                                                           model.rate: 0.6})
        print("step "+str(i)+",loss "+str(loss_value))

        if i % 10 == 0:
            # 计算训练集的准确率
            acc_train, summary = sess.run([model.accuracy, model.merged], feed_dict={model.X: batch_x_train,
                                                                                     model.Y: batch_y_train,
                                                                                     model.rate: 0})
            # 写入训练过程图
            writer_train.add_summary(summary, i)

            # 获取一个batch的验证图片和标签信息
            batch_x_valid, batch_y_valid = sess.run([image_batch_valid, label_batch_valid])

            # 计算验证集的准确率
            acc_valid, summary = sess.run([model.accuracy, model.merged], feed_dict={model.X: batch_x_valid,
                                                                                     model.Y: batch_y_valid,
                                                                                      model.rate: 0})
            writer_valid.add_summary(summary, i)

            print("step {},Train Accuracy {:.4f},Valid Accuracy {:.4f}".format(i, acc_train, acc_valid))

        if i % 2000 == 0:
            # 保存模型
            saver.save(sess, "model/model_level2.ckpt", global_step=i)
            print("Save the model Successfully")

    coord.request_stop()
    coord.join(threads)


if __name__ == '__main__':
    train()
