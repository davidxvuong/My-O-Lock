# Copyright (c) 2015  Niklas Rosenstein
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

from __future__ import print_function
import myo as libmyo; libmyo.init()
import time
import sys
import serial


def led_for_pose(pose):
    poseString = str(pose)
    return {
        '<Pose: rest>':2,
        '<Pose: wave_in>':3,
        '<Pose: wave_out>':4,
        '<Pose: fist>':5,
        '<Pose: double_tap>':6,
        '<Pose: fingers_spread>':7
    }.get(poseString, -1)

    def set_leds(ser, pose):
        pose_led = led_for_pose(pose)
        if pose_led == -1: return

        for i in xrange(2,8):
            state = int(not(i ^ pose_led))
            ser.write('{0} {1}\r'.format(i, "on" if state else "off"))

class Listener(libmyo.DeviceListener):
    """
    Listener implementation. Return False from any function to
    stop the Hub.
    """


    def __init__(self):
        super(Listener, self).__init__()
        self.emg_enabled = False
        self.orientation = None
        self.pose = libmyo.Pose.rest
        self.rssi = None
        self.locked = False
        self.arduino = serial.Serial('/dev/cu.usbmodem1413', 115200, timeout=.1)

        self.led_state = 0
        self.ready = False
        self.combo = ['<Pose: wave_in>',
            '<Pose: fist>',
            '<Pose: fingers_spread>',
            '<Pose: fist>',
            '<Pose: wave_out>']
        self.i = 0

    def __led_for_pose(self, pose):
        poseString = str(pose)
        return {
            '<Pose: rest>':2,
            '<Pose: wave_in>':3,
            '<Pose: wave_out>':4,
            '<Pose: fist>':5,
            '<Pose: double_tap>':6,
            '<Pose: fingers_spread>':7
        }.get(poseString, -1)


    def __set_leds(self,pose):
        pose_led = self.__led_for_pose(pose)
        if pose_led == -1: return

        for i in xrange(2,8):
            state = int(not(i ^ pose_led))
            self.arduino.write('{0} {1}\r'.format(i, "on" if state else "off"))
            #time.sleep(10)

    def output(self):

        sys.stdout.flush()
        poseString = str(self.pose)
        print(poseString)
        #finite state machine
        # if (self.arduino.readline() == "ready" or self.ready == True):
        #     self.ready = True

        if self.i == len(self.combo):
            self.i = 0
            self.ready = False
            self.arduino.write("success\r")
            # print("success")
            #send back success
            return
        print(poseString)
        #Ignore rest pose
        if (poseString == '<Pose: rest>'):
            return

        #if current pose is the same as the previous
        if (self.i > 0 and poseString == self.combo[self.i-1]):
            return

        if (poseString == self.combo[self.i]):
            #send success
            self.arduino.write('{0} {1}\r'.format(self.i+2, "on"))
            self.i += 1
            # self.ready = False
        else:
            self.i = 0
            for x in range(2, 7):
                self.arduino.write('{0} {1}\r'.format(x, "off"))
            # self.ready = False
            #send back failed
        # else:
        #     for x in range(0, 4):
        #         self.arduino.write('{0} {1}\r'.format(x, "off"))
        #     self.ready = False

        # parts = []
        # if self.orientation:
        #     for comp in self.orientation:
        #         parts.append(str(comp).ljust(15))
        # parts.append(str(self.pose).ljust(10))
        # parts.append('E' if self.emg_enabled else ' ')
        # parts.append('L' if self.locked else ' ')
        # parts.append(self.rssi or 'NORSSI')
        # print('\r' + ''.join('[{0}]'.format(p) for p in parts), end='')


        # self.arduino.write("sadaioj");
        # print(self.arduino.readline())

        # if (poseString == '<Pose: rest>'):
        #
        #     print('rest')
        # elif poseString == '<Pose: wave_in>':
        #     print('wave in')
        # elif poseString == '<Pose: wave_out>':
        #     print('wave out')
        # elif poseString == '<Pose: fist>':
        #     print('fist')
        # elif poseString == '<Pose: double_tap>':
        #     print('double_tap')
        # elif poseString == '<Pose: fingers_spread>':
        #     print('finger spread')
        # else:
        #     print(poseString)

        # arduino.write("Hello from python!")

    def on_connect(self, myo, timestamp):
        myo.vibrate('short')
        myo.vibrate('short')
        myo.request_rssi()

    def on_rssi(self, myo, timestamp, rssi):
        self.rssi = rssi
        self.output()

    def on_event(self, event):
        """
        Called before any of the event callbacks.
        """

    def on_event_finished(self, event):
        """
        Called after the respective event callbacks have been
        invoked. This method is *always* triggered, even if one of
        the callbacks requested the stop of the Hub.
        """

    def on_pair(self, myo, timestamp):
        """
        Called when a Myo armband is paired.
        """

    def on_disconnect(self, myo, timestamp):
        """
        Called when a Myo is disconnected.
        """

    def on_pose(self, myo, timestamp, pose):

        # if self.pose != pose:
        #     self.__set_leds(pose)

        if pose == libmyo.Pose.double_tap:
            myo.set_stream_emg(libmyo.StreamEmg.enabled)
            self.emg_enabled = True
        elif pose == libmyo.Pose.fingers_spread:
            myo.set_stream_emg(libmyo.StreamEmg.disabled)
            self.emg_enabled = False
        self.pose = pose
        self.output()

    def on_orientation_data(self, myo, timestamp, orientation):
        self.orientation = orientation
        self.output()

    def on_accelerometor_data(self, myo, timestamp, acceleration):
        pass

    def on_gyroscope_data(self, myo, timestamp, gyroscope):
        pass

    def on_unlock(self, myo, timestamp):
        self.locked = False
        self.output()

    def on_lock(self, myo, timestamp):
        self.locked = True
        self.output()

    def on_sync(self, myo, timestamp, arm, x_direction):
        pass

    def on_unsync(self, myo, timestamp):
        pass

    def on_emg(self, myo, timestamp, emg):
        pass


def main():
    hub = libmyo.Hub()
    hub.set_locking_policy(libmyo.LockingPolicy.none)
    hub.run(1000, Listener())

    # Listen to keyboard interrupts and stop the hub in that case.
    try:
        while hub.running:
            time.sleep(0.5)
    except KeyboardInterrupt:
        print("\nQuitting ...")
    finally:
        hub.stop(True)
    hub.shutdown()

if __name__ == '__main__':
    main()
