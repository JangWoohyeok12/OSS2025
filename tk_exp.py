import pymysql
import tkinter
import tkinter.messagebox as mbox

def insertData():
    connect = pymysql.connect(host='127.0.0.1', user='root',
                              password='041211', db='market_db', charset='utf8')
    cursor = connect.cursor()

    data1 = edt1.get(); data2 = edt2.get(); data3 = edt3.get(); data4 = edt4.get()
    data5 = edt5.get(); data6 = edt6.get(); data7 = edt7.get(); data8 = edt8.get()

    sql = 'insert into member values(%s, %s, %s, %s, %s, %s, %s, %s)'
    data = (data1, data2, data3, data4, data5, data6, data7, data8)

    cursor.execute(sql, data)

    connect.commit()
    connect.close()

    mbox.showinfo('성공', '1건 등록 완료')
    selectData()

def deleteData():

    connect = pymysql.connect(host='127.0.0.1', user='root',
                              password='041211', db='market_db', charset='utf8')
    cursor = connect.cursor()

    data1 = edt1.get(); data2 = edt2.get(); data3 = edt3.get(); data4 = edt4.get()
    data5 = edt5.get(); data6 = edt6.get(); data7 = edt7.get(); data8 = edt8.get()

    sql = "DELETE FROM member WHERE mem_id=%s and mem_name=%s and mem_number=%s and addr=%s and phone1=%s and phone2=%s and height=%s and debut_date=%s"
    data = (data1, data2, data3, data4, data5, data6, data7, data8)
    cursor.execute(sql, data)

    connect.commit()
    connect.close()

    mbox.showinfo("성공", f"{cursor.rowcount}건 삭제 완료")
    selectData()

def selectData():
    strData1, strData2, intData3, strData4, strData5, strData6, intData7, strData8 \
        = [], [], [], [], [], [], [], []

    strData1.append("사용자 ID"); strData2.append("사용자 이름"); intData3.append("그룹 인원 수")
    strData4.append("주소"); strData5.append("전화번호 앞자리"); strData6.append("전화번호 뒷자리")
    intData7.append("키"); strData8.append("데뷔 날짜")

    connect = pymysql.connect(host='127.0.0.1', user='root',
                              password='041211', db='market_db', charset='utf8')
    cursor = connect.cursor()
    cursor.execute('select ifnull(mem_id, "*"), ifnull(mem_name, "*"), ifnull(mem_number, "*"), ifnull(addr, "*"), ifnull(phone1, "*"), \
                    ifnull(phone2, "*"), ifnull(height, "*"), ifnull(debut_date, "*") from member order by mem_id')

    while(True):
        row = cursor.fetchone()
        if row == None: break
        strData1.append(row[0]); strData2.append(row[1]); intData3.append(row[2])
        strData4.append(row[3]); strData5.append(row[4]); strData6.append(row[5])
        intData7.append(row[6]); strData8.append(row[7])

    listData1.delete(0, 'end'); listData2.delete(0, 'end')
    listData3.delete(0, 'end'); listData4.delete(0, 'end')
    listData5.delete(0, 'end'); listData6.delete(0, 'end')
    listData7.delete(0, 'end'); listData8.delete(0, 'end')

    for item1, item2, item3, item4, item5, item6, item7, item8 \
        in zip(strData1, strData2, intData3, strData4, strData5, strData6, intData7, strData8):
        listData1.insert('end', item1); listData2.insert('end', item2); listData3.insert('end', item3)
        listData4.insert('end', item4); listData5.insert('end', item5); listData6.insert('end', item6)
        listData7.insert('end', item7); listData8.insert('end', item8)

    connect.commit()
    connect.close()

if __name__ == '__main__':
    window = tkinter.Tk()
    window.geometry('600x300')
    window.title('완전한 GUI 응용 프로그램')

    edtFrame = tkinter.Frame(window); edtFrame.pack()
    listFrame = tkinter.Frame(window); listFrame.pack(side='bottom', fill='both', expand=1)

    edt1 = tkinter.Entry(edtFrame, width=10); edt1.pack(side='left', padx=10, pady=10)
    edt2 = tkinter.Entry(edtFrame, width=10); edt2.pack(side='left', padx=10, pady=10)
    edt3 = tkinter.Entry(edtFrame, width=10); edt3.pack(side='left', padx=10, pady=10)
    edt4 = tkinter.Entry(edtFrame, width=10); edt4.pack(side='left', padx=10, pady=10)
    edt5 = tkinter.Entry(edtFrame, width=10); edt5.pack(side='left', padx=10, pady=10)
    edt6 = tkinter.Entry(edtFrame, width=10); edt6.pack(side='left', padx=10, pady=10)
    edt7 = tkinter.Entry(edtFrame, width=10); edt7.pack(side='left', padx=10, pady=10)
    edt8 = tkinter.Entry(edtFrame, width=10); edt8.pack(side='left', padx=10, pady=10)

    btninsert = tkinter.Button(edtFrame, text='입력', command=insertData)
    btninsert.pack(side='left', padx=10, pady=10)
    btnselect = tkinter.Button(edtFrame, text='조회', command=selectData)
    btnselect.pack(side='left', padx=10, pady=10)
    btndelete = tkinter.Button(edtFrame, text='삭제', command=deleteData)
    btndelete.pack(side='left', padx=10, pady=10)

    listData1 = tkinter.Listbox(listFrame, bg='beige'); listData1.pack(side='left', fill='both', expand=1)
    listData2 = tkinter.Listbox(listFrame, bg='beige'); listData2.pack(side='left', fill='both', expand=1)
    listData3 = tkinter.Listbox(listFrame, bg='beige'); listData3.pack(side='left', fill='both', expand=1)
    listData4 = tkinter.Listbox(listFrame, bg='beige'); listData4.pack(side='left', fill='both', expand=1)
    listData5 = tkinter.Listbox(listFrame, bg='beige'); listData5.pack(side='left', fill='both', expand=1)
    listData6 = tkinter.Listbox(listFrame, bg='beige'); listData6.pack(side='left', fill='both', expand=1)
    listData7 = tkinter.Listbox(listFrame, bg='beige'); listData7.pack(side='left', fill='both', expand=1)
    listData8 = tkinter.Listbox(listFrame, bg='beige'); listData8.pack(side='left', fill='both', expand=1)

    window.mainloop()