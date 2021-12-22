一个可以触发android SDK编译的web服务。包含一个django工程，和一个可执行程序。

1， 编译可执行程序，用于接受django服务的命令，并执行。
cd nerccmd_server
make
cd ..
sudo -i
./nerccmd

2，安装python2.7, 运行该web 服务。
pip install django
./go.sh

3，在浏览器输入 ip:11916/upload 上传zip包，即可出发编译

