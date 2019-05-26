"""
计算准确率文件，用于计算模型预测的准确率（test.py文件对图片标签和预测结果进行保存，此函数用于批量的计算）
"""
import conf

def accuracy_calculate():
    """
    读取conf.TEST_MAPPINGS文件（实际数字，预测数字），判断实习数字与预测数字是否匹配，若不匹配，将其打印
    :return: 返回准确率
    """
    i = 0
    correct = 0
    with open(conf.TEST_MAPPINGS, "r") as f:
        dataLine = f.readline().strip("\n")
        while dataLine != "":
            i += 1
            tmpList = dataLine.split(',')
            # print(tmpList)
            if tmpList[0] == tmpList[1]:
                correct += 1
            else:
                print("Number:", i, "False:", tmpList[1], "True:", tmpList[0], "Path:", tmpList[2])
            dataLine = f.readline().strip("\n")
        f.close()
    print("Accuracy:", correct / i)
    return correct / i


if __name__ == '__main__':
    accuracy_calculate()
