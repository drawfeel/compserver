from django.http import HttpResponse
from django.http import HttpResponseRedirect
from django.shortcuts import render
from threading import Timer, Thread, Event
import os.path
import datetime, time
import os
import socket

ZIP_UPLOAD_DIRECTION="/home/zhy123/upload"
COMPILE_DIRECTION="/home/zhy123/android10"
COMPILING_FLAG_PATH="/home/zhy123/out/compiling"
BLOCK_MODE = 'OFF'
LOCAL_PORT=9777

compile_list = list()

def upload(request):
    if request.method == 'POST':
        response = HttpResponse()
        response.status_code = 200
        if os.path.exists(COMPILING_FLAG_PATH):
            response.content = "already compiling..."
            return HttpResponse(response)
        obj = request.FILES.get('zip_url', None)
        if not obj:  
            response.content = "no valid file"
            return HttpResponse(response)
        if not obj.name.endswith(".zip"): 
            response.content = "no valid zip file"
            return HttpResponse(response)
        flag=request.POST.get('flag_selector').encode("utf-8")
        file = os.path.join(ZIP_UPLOAD_DIRECTION, obj.name)
        if os.path.isfile(file) == True:
            os.remove(file)
        time.sleep(0.2)
        destination = open(file,'wb+')
        for chunk in obj.chunks():
            destination.write(chunk)  
        destination.close()
        token = time.strftime("%Y%m%d%H%M%S", time.localtime())
        ret = start_compile_thread(token, flag, ZIP_UPLOAD_DIRECTION, obj.name, "200")
        response.content = "compiling... Please check ftp token %s" %(token)
        return  HttpResponse(response)
    return render(request, 'upload.html')

def start_compile_thread(token, flag, direction, name, respStr):
    #t = Thread(target=call_compile, args=(token, flag, direction, name))
    t = Thread(target=send_compile_msg_thread, args=(token, flag, direction, name))
    t.daemon = True
    t.start()
    if ('ON' == BLOCK_MODE) :
        t.join() # block mode
    return respStr

def send_compile_msg_thread(token, flag, direction, file_name):
    file = "%s/%s" %(direction, file_name)
    if in_compile_list(token):
        return
    add_to_compile_list(token)
    try:
        socket.setdefaulttimeout(5)
        s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        s.connect(("localhost", LOCAL_PORT))
        msg = "cd %s;" %(COMPILE_DIRECTION)
        bash_cmd = "bash makeo.sh %s %s" %(file, token)
        if ("1" == flag):
            bash_cmd = "bash makeoall.sh %s %s" %(file, token)
        msg += bash_cmd;
        s.send(msg.encode("utf-8"))
        s.close()
    except Exception as ex:
        print(ex)
        
    remove_from_compile_list(token)

def call_compile(token, flag, direction, file_name):
    file = "%s/%s" %(direction, file_name)
    if in_compile_list(token):
        return HttpResponse("already compiling %s" %(file_name))
    add_to_compile_list(token)

    time.sleep(0.2)
    a = os.chdir(COMPILE_DIRECTION)
    cmd = "bash makeo.sh %s %s" %(file, token)
    if ("1" == flag):
        cmd = "bash makeoall.sh %s %s" %(file, token)
    a = os.system(cmd)

    if(0 == a):
        remove_from_compile_list(token)

def remove_from_compile_list(item):
    flist = get_compile_list()
    if item in flist:
        del flist[flist.index(item)]

def add_to_compile_list(item):
    flist = get_compile_list()
    flist.append(item)
    
def get_compile_list():
    global compile_list
    return compile_list
    
def in_compile_list(item):
    flist = get_compile_list()
    return item in flist
