# person_pose_estimator

This project is used as a helper for hospital to know the status of each mental patient. It can detect patient's pose in real-time, because we place many known zigbee labels in each rooms and equip with a sensor for every patient. As the abnormal circumstance occurs, the nurse can find those changes from the monitor screen system. For web, we use flask as our development framework, and zigbee network is used in pi instead.

Now, Let's start to introduce the file structure of our project. Generally, our project is divided into three parts, with [micro-controller](Micro-controller/README.md) (sensors, receivers and et. al.), Gateway(pi), and Server(web server).


## Get Started

### Pi

```
git clone https://github.com/ZhuChaozheng/person_pose_estimator
cd person_pose_estimator
python3 Pi/zigbee_receiver.py > ~/log.txt
```

also modify the server IP in *zigbee_receiver.py*

Now, we have finished the deloyment of hardware layer. Next, Let us consider the web. As you can see, we have a folder of web, you should move it to your web server.

```
scp -r Server/web/ blackant@192.168.1.102:/var/www/html
```

### Ubuntu server

the following operations please switch to your web server

```
cd /var/www/html/web/
python3 manange.py
```
