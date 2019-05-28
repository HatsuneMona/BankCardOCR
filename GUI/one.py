import tkinter as tk
from tkinter import *
from tkinter import filedialog
from PIL import Image, ImageTk
from tkinter import ttk





# 窗口初始化
frame = tk.Tk()
frame.title("银行卡卡号识别程序")
frame.resizable(False, False)
screenwidth = frame.winfo_screenwidth()
screenheight = frame.winfo_screenheight()
x = (screenwidth - 1000) / 2
y = (screenheight - 600) / 2
frame.geometry("%dx%d+%d+%d" % (1000, 600, x, y))
result = StringVar(frame, value='请选择银行卡图片进行识别')
# 主窗体背景图绘制
bg = Image.open('bg3.jpg')
bg1 = bg.resize((1005, 600))
bg1photo = ImageTk.PhotoImage(bg1, width=1002, height=600)
canvas = tk.Canvas(frame, width=1002, height=600)
canvas.create_image(500, 300, image=bg1photo)
# "银行卡卡号识别"文字绘制
canvas.create_text(505, 50, text='银行卡卡号识别', fill='white', font=("黑体", 25, "bold"))
# "识别结果"文字绘制
canvas.create_text(100, 500, text='识别结果：', fill='white', font=("黑体", 15, "bold"), tags='a')
# 用户选择图片标签绘制
selectimg = Image.open('bg4.jpg')
selectimg1 = selectimg.resize((600, 400))
selectphoto = ImageTk.PhotoImage(selectimg1, width=600, height=400)
selectlabel = tk.Label(frame, image=selectphoto)
canvas.create_window(350, 270, width=600, height=350, window=selectlabel, tags='selectlabel')


# 开始识别事件
def callback():
    global result
    print('正在识别...（普通模式）')
    result.set('正在识别，请稍后...')

    print('识别完成！')


# 开始识别事件
def callback2():
    print('开始识别...（高级模式）')
    print('识别完成！')

# 选择图片事件
def select():
    filename = filedialog.askopenfilename(title='请选择图片文件', filetypes=[("Image files", "*.png;*.jpg;*.bmp;*.JPG")])
    print(filename)
    img2 = Image.open(filename)
    img3 = img2.resize((800, 500))
    photo2 = ImageTk.PhotoImage(img3, width=800, height=400)
    selectlabel.config(image=photo2)
    selectlabel.image = photo2  # keep a reference！
    return img2


# 选择文件目录（高级模式）
def select2():
    filename = filedialog.askdirectory()
    print(filename)


# 开始识别按钮绘制（普通模式）
startbtn = tk.Button(frame, text="开始识别", width=100, height=50, command=callback, font=("黑体", 15, "bold"),
                     relief='flat', bg="#3399ff", fg='white')
canvas.create_window(835, 280, width=100, height=50, window=startbtn, tags="startbtn")
# 图片选择按钮绘制（普通模式）
selectbtn = tk.Button(frame, text="选择图片", command=select, font=("黑体", 15, "bold"), width=100,
                      height=50, relief='flat', bg="#ff3399", fg='white')
canvas.create_window(835, 180, width=100, height=50, window=selectbtn, tags='selectbtn')


# 开始识别按钮绘制（高级模式）
startbtn2 = tk.Button(frame, text="开始识别", width=100, height=50, command=callback2, font=("黑体", 15, "bold"),
                     relief='flat', bg="#3399ff", fg='white')
# 文件目录选择按钮绘制（高级模式）
selectbtn2 = tk.Button(frame, text="选择目录", command=select2, font=("黑体", 15, "bold"), width=100,
                      height=50, relief='flat', bg="#ff3399", fg='white')


# 退出按钮绘制
quitbtn = tk.Button(frame, text="退出", command=frame.quit, font=("黑体", 13, "bold"), width=70,
                    height=30, relief='flat', bg="#ff9933", fg='white')
canvas.create_window(950, 570, width=70, height=30, window=quitbtn)
# 识别结果显示标签绘制
resultlabel = tk.Entry(frame, textvariable=result, relief='flat', bg="#ff99ff", font=("黑体", 20, "bold"),
                       fg='white', selectbackground='#3399ff')
canvas.create_window(400, 500, width=500, height=50, window=resultlabel, tags='resultlabel')

# 文件列表表格
filetree = ttk.Treeview(frame, columns=('1', '2', '3'), show="headings")
filetree.column('1', width=200, anchor='center')
filetree.column('2', width=100, anchor='center')
filetree.column('3', width=290, anchor='center')
filetree.heading('1', text="图片名称")
filetree.heading('2', text="略缩图")
filetree.heading('3', text="识别结果")
#垂直滚动条
scrollBar = tk.Scrollbar(filetree, orient=VERTICAL)
scrollBar.pack(side=tk.RIGHT, fill=tk.Y)
scrollBar.config(command=filetree.yview)
filetree.config(yscrollcommand=scrollBar.set)


# 高级模式
def advencemodel():
    global canvas
    canvas.delete('a', 'startbtn', 'selectbtn', 'resultlabel', 'advencebtn', 'selectlabel')
    canvas.create_window(835, 280, width=100, height=50, window=startbtn2, tags="startbtn2")
    canvas.create_window(835, 180, width=100, height=50, window=selectbtn2, tags='selectbtn2')
    canvas.create_window(835, 380, width=100, height=50, window=easybtn, tags='easybtn')
    canvas.create_window(350, 270, width=600, height=350, window=filetree, tags='filetree')


# 返回普通模式
def easymodel():
    global canvas
    canvas.delete('startbtn2', 'selectbtn2', 'easybtn', 'filetree')
    canvas.create_text(100, 500, text='识别结果：', fill='white', font=("黑体", 15, "bold"), tags='a')
    canvas.create_window(835, 280, width=100, height=50, window=startbtn, tags="startbtn")
    canvas.create_window(835, 180, width=100, height=50, window=selectbtn, tags='selectbtn')
    canvas.create_window(400, 500, width=500, height=50, window=resultlabel, tags='resultlabel')
    canvas.create_window(835, 380, width=100, height=50, window=advencebtn, tags='advencebtn')
    canvas.create_window(350, 270, width=600, height=350, window=selectlabel, tags='selectlabel')


# 高级模式按钮绘制
advencebtn = tk.Button(frame, text="高级模式", command=advencemodel, font=("黑体", 15, "bold"), width=100,
                       height=50, relief='flat', bg="#ff9933", fg='white')
canvas.create_window(835, 380, width=100, height=50, window=advencebtn, tags='advencebtn')

# 普通模式按钮绘制
easybtn = tk.Button(frame, text="普通模式", command=easymodel, font=("黑体", 15, "bold"), width=100,
                       height=50, relief='flat', bg="#ff9933", fg='white')
# 画布位置坐标
canvas.place(x=-2, y=0)
# 主窗体显示
frame.mainloop()
