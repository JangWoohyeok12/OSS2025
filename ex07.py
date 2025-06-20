import pymysql
import tkinter
import tkinter.messagebox as mbox

from twisted.conch.insults.window import cursor

###############################---Insert---#################################

def insertData():

    connect = pymysql.connect(host='127.0.0.1', user='root',
                              password='041211',db='shop_db', charset='utf8')
    cursor= connect.cursor()

    data1 = edt1.get(); data2 = edt2.get(); data3 = edt3.get(); data4 = edt4.get()
    sql = "INSERT INTO usertable VALUES(%s, %s, %s, %s)"
    data = (data1, data2, data3, data4)
    cursor.execute(sql, data)

    connect.commit()
    connect.close()

    mbox.showinfo('성공', '1건 등록 완료')
    selectData()

###############################---Select---#################################

def selectData() :

    strData1, strData2, strData3, strData4 = [], [], [], []
    strData1.append("사용자 ID"); strData2.append("사용자 이름")
    strData3.append("이메일"); strData4.append("출생연도 (숫자)")
    strData1.append("-------------"); strData2.append("-------------")
    strData3.append("-------------"); strData4.append("-------------")

    connect = pymysql.connect(host='127.0.0.1', user='root', password='1234', db='shop_db', charset='utf8')
    cursor = connect.cursor()
    cursor.execute("SELECT * FROM userTable order by userid")

    while (True) :
        row = cursor.fetchone()
        if row== None : break;
        strData1.append(row[0]); strData2.append(row[1]); strData3.append(row[2]); strData4.append(row[3])

    listData1.delete(0,listData1.size() - 1); listData2.delete(0,listData2.size() - 1)
    listData3.delete(0,listData3.size() - 1); listData4.delete(0,listData4.size() - 1)

    for item1, item2, item3, item4 in zip(strData1, strData2, strData3, strData4 ):
        listData1.insert('end', item1); listData2.insert('end', item2)
        listData3.insert('end', item3); listData4.insert('end', item4)

    connect.close()

########################---MAIN---###########################

window = tkinter.Tk()
window.geometry("600x300")
window.title('완전한 GUI 응용 프로그램')

edtFrame = tkinter.Frame(window); edtFrame.pack()
listFrame = tkinter.Frame(window); listFrame.pack(side ='bottom',fill='both', expand=1)

edt1= tkinter.Entry(edtFrame, width=10); edt1.pack(side='left',padx=10,pady=10)
edt2= tkinter.Entry(edtFrame, width=10); edt2.pack(side='left',padx=10,pady=10)
edt3= tkinter.Entry(edtFrame, width=10); edt3.pack(side='left',padx=10,pady=10)
edt4= tkinter.Entry(edtFrame, width=10); edt4.pack(side='left',padx=10,pady=10)

btnInsert = tkinter.Button(edtFrame, text="입력", command = insertData)
btnInsert.pack(side='left',padx=10,pady=10)
btnSelect = tkinter.Button(edtFrame, text="조회", command =selectData )
btnSelect.pack(side='left',padx=10,pady=10)

listData1 = tkinter.Listbox(listFrame,bg = 'yellow'); listData1.pack(side='left',fill='both', expand=1)
listData2 = tkinter.Listbox(listFrame,bg = 'yellow'); listData2.pack(side='left',fill='both', expand=1)
listData3 = tkinter.Listbox(listFrame,bg = 'yellow'); listData3.pack(side='left',fill='both', expand=1)
listData4 = tkinter.Listbox(listFrame,bg = 'yellow'); listData4.pack(side='left',fill='both', expand=1)

window.mainloop()