"""
模型类
"""
import tensorflow as tf

# 输入长宽高
width_input = 39;
hight_input = 46;
depth_input = 1;
# 输出长宽高
width_output = 10;
hight_output = 12;
depth_output = 64;


class Model(object):
    def __init__(self):
        with tf.name_scope('input'):
            # 初始化X，即输入图片的信息格式[不知数量，长44，宽60，深度1]
            self.X = tf.placeholder(tf.float32, [None, width_input, hight_input, depth_input], name='X')
            # 初始化Y，即输出信息
            self.Y = tf.placeholder(tf.float32, [None], name='Y')
            # 标签
            label = tf.one_hot(indices=tf.cast(self.Y, tf.int32), depth=11, name='y_onehot')
            # 弃用神经元数量
            self.rate = tf.placeholder(tf.float32, name='rate')
        # 预测值（model函数的返回值）
        self.prediction = self.model()

        with tf.name_scope('output'):
            with tf.name_scope('correct_prediction'):
                # 判断是否准确，返回值是一个Bool
                correct_prediction = tf.equal(tf.argmax(self.prediction, 1), tf.argmax(label, 1))
            with tf.name_scope('loss'):
                # 损失计算
                self.loss = tf.reduce_mean(
                    tf.nn.softmax_cross_entropy_with_logits_v2(logits=self.prediction, labels=label))
                tf.summary.scalar('loss', self.loss)
            with tf.name_scope('AdamOptimizer'):
                # 使用GradientDescent梯度下降，学习了为0.03
                self.optimizer = tf.train.GradientDescentOptimizer(0.13).minimize(self.loss)
            with tf.name_scope('accuracy'):
                # 将Bool值的是否准确转化为float形式，然后计算正确的样本占比（正确率）
                self.accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
                tf.summary.scalar('accuracy', self.accuracy)
        self.merged = tf.summary.merge_all()

    def model(self):# 模型函数

        with tf.variable_scope("C1-conv"):
            # 卷积层,卷积核为5*5，步长为[1,1]
            conv1_weights = tf.get_variable("weight", [5, 5, depth_input, 32])
            conv1_biases = tf.get_variable("bias", [32])
            conv1 = tf.nn.conv2d(self.X, conv1_weights, strides=[1, 1, 1, 1], padding="SAME")# 卷积层的计算，边缘用0填充，输出加深深度，从1到64
            relu1 = tf.nn.relu(tf.nn.bias_add(conv1, conv1_biases))# 对上一个数据进行relu函数的处理
        # 39*46*32
        with tf.variable_scope("S2-max_pool"):
            # 最大池化层,卷积核为2*2，步长为[2,2]
            pool1 = tf.nn.max_pool(relu1, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding="SAME")
            # drop_s2 = tf.nn.dropout(pool1, rate=self.rate, name='drop_c2')
        # 20*23*32

        with tf.variable_scope("C3-conv"):
            # 卷积层,卷积核为5*5，步长为[1,1]
            conv2_weights = tf.get_variable("weight", [5, 5, 32, 64])
            conv2_biases = tf.get_variable("bias", [64])
            conv2 = tf.nn.conv2d(pool1, conv2_weights, strides=[1, 1, 1, 1], padding="SAME")
            relu2 = tf.nn.relu(tf.nn.bias_add(conv2, conv2_biases))
        # 20*23*64

        with tf.variable_scope("S4-max_pool"):
            # 最大池化层,卷积核为2*2，步长为[2,2]
            pool2 = tf.nn.max_pool(relu2, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding="SAME")
            drop_s4 = tf.nn.dropout(pool2, rate=self.rate, name='drop_c2')
        # 10*12*64

        with tf.variable_scope("layer5-full1"):
            # 全连接层
            dense = tf.reshape(drop_s4, [-1, width_output*hight_output*depth_output], name='dense')
            full1_weights = tf.Variable(0.1*tf.random_normal([width_output*hight_output*depth_output, 512]), name='w_fc1')
            full1_biases = tf.Variable(0.1*tf.random_normal([512]), name='b_fc1')

            full_1 = tf.nn.relu(tf.matmul(dense, full1_weights) + full1_biases)
            Drop1 = tf.nn.dropout(full_1, rate=self.rate)

        with tf.name_scope('layer6-full1'):
            # 第二层全连接层 输出为 1*11
            full3_weights = tf.Variable(0.01 * tf.random_normal([512, 11]), name='w_fc2')
            full3_biases = tf.Variable(0.01 * tf.random_normal([11]), name='b_fc2')
            result = tf.add(tf.matmul(Drop1, full3_weights), full3_biases, name='wx_plus_b')

        return result


if __name__ == '__main__':
    model = Model()
