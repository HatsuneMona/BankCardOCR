"""
将jpg图片以及标签生成TFRecord文件格式保存，形式为image label
"""
import tensorflow as tf
import os
import random
import sys
from PIL import Image
import numpy as np
import conf


def _get_filenames_and_classes(dataset_dir):
    """
    dataset_dir：文件夹路径
    获取图片路径以及所在的文件夹名（标签）
    """
    directories = []# 文件夹路径名
    class_names = []# 文件夹名
    for filename in os.listdir(dataset_dir):
        path = os.path.join(dataset_dir, filename)
        if os.path.isdir(path):# 如果path是一个文件夹，将这个路径加入到directories中，文件夹名添加到class_names中
            directories.append(path)
            class_names.append(filename)
    image_filenames = []

    for directory in directories:# 在文件夹中遍历，将图片的路径增加到image_filenames中
        for filename in os.listdir(directory):
            path = os.path.join(directory, filename)
            image_filenames.append(path)
    # 返回值为图片的路径和所在的文件夹名的两个List
    return image_filenames, class_names


def int64_feature(values):
    if not isinstance(values, (tuple, list)):
        values = [values]
    return tf.train.Feature(int64_list=tf.train.Int64List(value=values))


def bytes_feature(values):
    return tf.train.Feature(bytes_list=tf.train.BytesList(value=[values]))


def image_to_tfexample(image_data, class_id):
    return tf.train.Example(features=tf.train.Features(feature={
        'image': bytes_feature(image_data),
        'label': int64_feature(class_id),
    }))


def write_label_file(labels_to_class_names, labels_filename):
    """ Write value-label into labels.txt """
    with tf.gfile.Open(labels_filename, "w") as f:
        for label in labels_to_class_names:
            class_name = labels_to_class_names[label]
            f.write('%d:%s\n' % (label, class_name))


def _convert_dataset(split_name, filenames, class_names_to_ids, dataset_dir):
    """
    生成tfrecord文件并将图片信息保存至生成的tfrecord文件中
    split_name:生成的文件名，有image_train.tfrecord和image_valid.tfrecord
    filenames:图片文件路径列表
    class_names_to_ids:集合，key为标签名，value为索引
    dataset_dir:保存数据文件夹的地址
    """
    assert split_name in ['train', 'valid', 'test']

    output_filename = os.path.join(dataset_dir, "image_%s.tfrecord" % split_name)# 拼接路径
    tfrecord_writer = tf.python_io.TFRecordWriter(output_filename)# 在output_filename路径下面生成tfrecoed格式文件
    for i in range(len(filenames)):
        image_data = Image.open(filenames[i])
        image_data = np.array(image_data.convert('L'))# 将图片转化为矩阵形式
        image_data = image_data.tobytes()# 将图片数据转化为字节形式
        class_name = os.path.basename(os.path.dirname(filenames[i]))# 获取读取的图片所在文件夹名
        class_id = class_names_to_ids[class_name]# 获取读取的图片所在文件夹名的id
        example = image_to_tfexample(image_data, class_id)# 存储图片数据形式
        tfrecord_writer.write(example.SerializeToString())
        sys.stdout.write('\r>> Converting %s_image %d/%d' % (split_name, i+1, len(filenames)))
        sys.stdout.flush()

    sys.stdout.write('\n')
    sys.stdout.flush()


if __name__ == '__main__':
    # 获取图片路径以及所在文件夹路径的列表，所在文件夹相当于图片的标签，相同标签的图片存放于一个文件夹中
    image_filenames_train, class_names_train = _get_filenames_and_classes(conf.CUT_PATH_TRAIN)
    image_filenames_test, class_names_test = _get_filenames_and_classes(conf.CUT_PATH_TEST)
    image_filenames_valid, class_names_valid = _get_filenames_and_classes(conf.CUT_PATH_VALID)
    # 将文件夹名转化为字典{文件夹名：id}，id对应着文件夹名在列表的索引，用于预测数字时的转换
    class_names_to_ids_train = dict(zip(class_names_valid, range(len(class_names_valid))))# 返回class_names：索引的集合
    class_names_to_ids_test = dict(zip(class_names_valid, range(len(class_names_valid))))# 返回class_names：索引的集合
    class_names_to_ids_valid = dict(zip(class_names_valid, range(len(class_names_valid))))# 返回class_names：索引的集合
    # 打乱列表
    random.seed(0)
    random.shuffle(image_filenames_train)
    random.shuffle(image_filenames_test)
    random.shuffle(image_filenames_valid)
    # 将数据集和验证机分别转化成TFRECORD格式文件
    _convert_dataset('valid', image_filenames_valid, class_names_to_ids_valid, conf.TFRECORD_PATH)
    _convert_dataset('test', image_filenames_test, class_names_to_ids_test, conf.TFRECORD_PATH)
    _convert_dataset('train', image_filenames_train, class_names_to_ids_train, conf.TFRECORD_PATH)
