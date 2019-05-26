import tkinter as tk
from tkinter import filedialog
from PIL import Image, ImageTk
import test
import tensorflow as tf
from model import Model
import conf


# 开始识别
def callback(model, sess):
    print('正在识别...')
    label03.config(text="正在识别，请稍等...")
    print(filename)
    content = test.test4(model, sess, filename)
    label03.config(text=content)
    # print('识别完成！')


# 选择图片
def select():
    global filename
    filename = filedialog.askopenfilename(title='请选择图片文件', filetypes=[("Image files", "*.png;*.jpg;*.bmp;*.JPG")])
    # print(filename)
    img2 = Image.open(filename)
    img3 = img2.resize((800, 500))
    photo2 = ImageTk.PhotoImage(img3, width=800, height=400)
    label02.config(image=photo2)
    label02.image = photo2  # keep a reference！
    return img2


if __name__ == '__main__':
    # 窗口初始化
    frame = tk.Tk()
    frame.title("银行卡卡号识别程序")
    frame.resizable(False, False)
    screenwidth = frame.winfo_screenwidth()
    screenheight = frame.winfo_screenheight()
    x = (screenwidth - 800) / 2
    y = (screenheight - 600) / 2
    frame.geometry("%dx%d+%d+%d" % (800, 600, x, y))

    # 识别结果提示显示标签
    label01 = tk.Label(frame, text="识别结果：", font=("黑体", 20, "bold"))
    label01.place(x=20, y=520)

    # 图片显示标签
    img = Image.open('img.jpg')
    img1 = img.resize((800, 500))
    photo = ImageTk.PhotoImage(img1, width=800, height=400)
    label02 = tk.Label(frame, text="选择图片", image=photo)
    label02.place(x=0, y=0)

    # 识别结果显示标签
    label03 = tk.Label(frame, text="", font=("黑体", 15, "bold"))
    label03.place(x=150, y=525)
    # 导入模型
    model = Model()
    saver = tf.train.Saver()
    sess = tf.Session()
    sess.run(tf.global_variables_initializer())
    saver.restore(sess, conf.MODEL_PATH)

    button2 = tk.Button(frame, text="选择图片", command=select)
    button2.place(x=500, y=520)
    button = tk.Button(frame, text="开始识别", command=lambda: callback(model=model, sess=sess))
    button.place(x=700, y=520)
    frame.mainloop()

