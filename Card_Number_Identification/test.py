import sys
import tensorflow as tf
from model import Model
import cv2
import conf
import os
from train import parse_data

step_test = 36
# 索引转化为预测值的字典
num2str = {
    0: "0", 1: "1", 2: "2", 3: "3", 4: "4", 5: "5", 6: "6", 7: "7", 8: "8",
    9: "9", 10: "_"
}


def test(model, sess):
    """
    :param model:模型对象
    :param sess:会话
    :return:None
    用于读取conf.TEST_IMAGE_PATH路径下的图片，并将预测出来的数字保存到conf.MAPPINGS文件中
    （保存形式为：实际数字，预测数字）路径下的图片为120*46*3，共有4个数字
    """
    result = tf.argmax(model.prediction, 1)
    f = open(conf.TEST_MAPPINGS, "w")
    i = 1
    for filename in os.listdir(conf.TEST_IMAGE_PATH):
        # 灰度读取图片
        image = cv2.imread(conf.TEST_IMAGE_PATH+"/"+filename, cv2.IMREAD_GRAYSCALE)
        # 裁剪图片
        cut_list = [image[:, 0:39], image[:, 27:66], image[:, 54:93], image[:, 81:120]]

        # 图片处理
        for j in range(len(cut_list)):
            cut_list[j] = cut_list[j].reshape([39, 46, 1])/255.0

        # 获取预测值
        num_list = sess.run(result, feed_dict={model.X: cut_list, model.rate: 0})  # 传入模型进行预测
        prediction_list = [num2str[num] for num in num_list]
        # 将实际值与预测值写入文件
        f.write(filename[0:4]+","+"".join(prediction_list)+","+filename+"\n")
        sys.stdout.write('\r>> Testing image %d/%d'%(i, len(os.listdir(conf.TEST_IMAGE_PATH))))
        sys.stdout.flush()
        i += 1


def test2(model, sess):
    """
    :param model:模型对象
    :param sess:会话
    :return:None
    用于读取conf.TEST_IMAGE_PATH路径下的图片，并打印预测出来的数字（List），路径下的图片为120*46*3，共有4个数字
    """
    result = tf.argmax(model.prediction, 1)
    prediction_list = []
    for filename in os.listdir(conf.TEST_IMAGE_PATH):
        image = cv2.imread(conf.TEST_IMAGE_PATH+"/"+filename, cv2.IMREAD_GRAYSCALE)
        cut_list = [image[:, 0:39], image[:, 27:66], image[:, 54:93], image[:, 81:120]]
        # cut_list = [image[:, 0:39]]
        for j in range(len(cut_list)):
            cut_list[j] = cut_list[j].reshape([39, 46, 1]) / 255.0
        num_list = sess.run(result, feed_dict={model.X: cut_list, model.rate: 0})
        prediction_list = [num2str[num] for num in num_list]
        print(prediction_list, filename[0:4])

    return prediction_list


def test3(model, sess):
    """
    :param model:模型对象
    :param sess:会话
    :return:None
    batch_size大小为1024
    step_test:表示循环的batch_size次数
    用于读取conf.TFRECORD_TEST_PATH路径下的TFRECODER格式文件并计算准确率
    """
    dataset_test = tf.data.TFRecordDataset(conf.TFRECORD_TEST_PATH)
    dataset_test = dataset_test.map(parse_data).shuffle(36000).batch(1000)
    iterator_train = dataset_test.make_initializable_iterator()
    image_batch_test, label_batch_test = iterator_train.get_next()
    sess.run(iterator_train.initializer)
    # 多线程
    coord = tf.train.Coordinator()
    threads = tf.train.start_queue_runners(sess=sess, coord=coord)
    acc = 0
    for i in range(step_test):
        # 获取一个batch的验证图片和标签信息
        batch_x_test, batch_y_test = sess.run([image_batch_test, label_batch_test])
        # 计算验证集的准确率
        acc_test = sess.run([model.accuracy], feed_dict={model.X: batch_x_test,
                                                                        model.Y: batch_y_test,
                                                                        model.rate: 0})
        acc = acc + acc_test[0]
    # 打印测试集准确率
    print(acc/step_test)
    coord.request_stop()
    coord.join(threads)


def test4(model, sess, filename):
    """
    :param model:模型对象
    :param sess:会话
    :return:识别的数字（字符串形式）
    用于读取conf.TEST_IMAGE_PATH路径下的图片，并打印预测出来的数字（List），路径下的图片为120*46*3，共有4个数字
    """
    result = tf.argmax(model.prediction, 1)
    image = cv2.imread(filename, cv2.IMREAD_GRAYSCALE)
    # cut_list = [image[0:46, 0:39], image[0:46, 27:66], image[0:46, 54:93], image[0:46, 81:120]]
    cut_list = [image[:, 0:39], image[:, 27:66], image[:, 54:93], image[:, 81:120]]
    # cut_list = [image[:, 0:39]]
    for j in range(len(cut_list)):
        cut_list[j] = cut_list[j].reshape([39, 46, 1]) / 255.0
    num_list = sess.run(result, feed_dict={model.X: cut_list, model.rate: 0})
    prediction_list = [num2str[num] for num in num_list]
    prediction = "".join(prediction_list).replace("_", "")
    print(prediction_list)
    print(prediction)
    return prediction


if __name__ == '__main__':
    model = Model()
    saver = tf.train.Saver()
    sess = tf.Session()
    sess.run(tf.global_variables_initializer())
    saver.restore(sess, conf.MODEL_PATH)
    # test(model, sess)
    # test2(model, sess)
    test3(model, sess)
    # test4(model, sess, r'E:\python\learning_machine\level2\images\test\_001a_0.png')

