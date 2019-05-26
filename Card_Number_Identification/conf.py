"""
路径的配置
"""
# 模型路径
MODEL_PATH = "./model/model_level2.ckpt-20000"
# 测试图片路径
TEST_IMAGE_PATH = './images/test'
# TEST_IMAGE_PATH = r'C:\Users\Whether\Desktop\images\images'
# 裁切，处理好（变化过）的图片路径（单个数字）
CUT_PATH_TRAIN = r"C:\Users\Whether\Desktop\images\train"
CUT_PATH_TEST = r"C:\Users\Whether\Desktop\images\test"
CUT_PATH_VALID = r"C:\Users\Whether\Desktop\images\valid"
# CUT_PATH = "./images/train"
# path of train images
TRAIN_IMAGE_PATH = "./images/train"
# TFRecord格式文件保存路径
TFRECORD_PATH = "./tfrecord"
# 训练TFRECORD文件保存路径
TFRECORD_TRAIN_PATH = "./tfrecord/image_train.tfrecord"
# 验证TFRECORD文件保存路径
TFRECORD_VALID_PATH = "./tfrecord/image_valid.tfrecord"
# 测试TFRECORD文件保存路径
TFRECORD_TEST_PATH = "./tfrecord/image_test.tfrecord"
# test_mappings.txt路径（保存测试的 实际数字：预测数字）
TEST_MAPPINGS = "./mappings/test_mappings.txt"
