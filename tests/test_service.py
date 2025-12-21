# sub.py
import zmq
import json
import time

ctx = zmq.Context()
pub_sock = ctx.socket(zmq.PUB)
pub_sock.connect("tcp://0.0.0.0:2808")
sub_sock = ctx.socket(zmq.SUB)
sub_sock.connect("tcp://0.0.0.0:2809")  # XPUB

sub_sock.setsockopt_string(zmq.SUBSCRIBE, "disc")

print("[SUB] waiting...")
while True:
    topic, header = sub_sock.recv_multipart()
    print("[SUB] received:", topic, header)

    desc = {}

    desc["name"] = "Test Application"
    desc["compatible_protocols"] = "JSON"
    desc["version"] = "1.0"

    desc["commands"] = []

    res_payload = json.dumps(desc)

    pub_sock.send(res_payload.encode('ascii'))
    print("[PUB] sent:", res_payload)
    time.sleep(1)


sock.close()
ctx.term()
