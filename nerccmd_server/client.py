import socket

class Client():
    def send_msg(self):
        LOCAL_PORT=9777
        goon=True
        try:
            socket.setdefaulttimeout(5)
            s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
            s.connect(("localhost", LOCAL_PORT))
            if(goon):
                msg = "cd /home/zhy123/android10;bash makeoall.sh /home/zhy123/upload/2223.zip 333"
                s.send(msg.encode("utf-8"))
#                data=s.recv(256)
#                if len(data) > 0 :
#                    print("recv:>", data.decode("utf-8"))
                print("BYE ...")
                s.close()
        except Exception as ex:
            print(ex)

if __name__ == "__main__":
    client=Client()
    client.send_msg()
