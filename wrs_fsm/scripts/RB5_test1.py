#!/usr/bin/env python

import rospy
import smach
import smach_ros
from smach import StateMachine
import time
import math


#import action headers
import actionlib
import wrs_fsm.msg
from wrs_fsm.msg import *
import mobile_path_planning.msg
from mobile_path_planning.msg import *


# Global variables====================
# Shelf selecting
item_types = 6                  #2 for now for test #6          # TEST SETTING
item_order = [2,3,4,5,0,1]      # TEST SETTING
item_count = 0

# action setting for motion_director
motion_cmd = 0
motion_CMD = {'MOTION_NONE':0, 'MOTION_GRASPING':1, 'MOTION_TEST':2, 'MOTION_SHELFOUT':3, 'MOTION_SHELFIN':4, 'MOTION_DISPOSE':5 ,'MOTION_DETECT_ONSHELF':6, 'MOTION_ALIGN_OLD':7, 'MOTION_SEARCH_NEW':8, 'MOTION_DISPLAY_NEW':9, 'MOTION_HOME_POSE':10}

#flag
motion_direction_result_flag = False
navi_result_flag = False
pause_flag = False
skipLine_flag = True

# state states
state_result = True

# special case:
new_display_cnt = 0
shelf_out_fail_cnt = 0
shelfout_recovery_mode = False

# shelf_num and object_id
shelf_num = 0
object_id = 0
# ========================================

# ================================================ Result Callback ================================================ #
def motion_direction_result_callback(data):
    global motion_direction_result_flag
    global state_result
    global skipLine_flag

    rospy.loginfo("motion_direction Callback result flag: %i", data.result.result_flag)
    state_result = data.result.result_flag
    if(state_result == 2):
        skipLine_flag = True
        print "skipLine_flag On"
    motion_direction_result_flag = True

def navi_result_callback(data):
    global navi_result_flag
    global navi_pose_x, navi_pose_y, navi_pose_z
    global navi_ori_x, navi_ori_y, navi_ori_z, navi_ori_w
    global state_result

    rospy.loginfo("navi Callback result flag: %i", data.result.result_flag)
    navi_result_flag = True
    state_result = data.result.result_flag

    #data update
    navi_pose_x = data.result.pose_x
    navi_pose_y = data.result.pose_y
    navi_pose_z = data.result.pose_z
    navi_ori_x = data.result.ori_x
    navi_ori_y = data.result.ori_y
    navi_ori_z = data.result.ori_z
    navi_ori_w = data.result.ori_w

# ================================================ state machine states ================================================ #

# define state IDLE
class IDLE(smach.State):
    def __init__(self):
        smach.State.__init__(self, outcomes=['outcome1', 'outcome2'])

    def execute(self, userdata):
        print(" === Starting Store Application Motion ===")
        time.sleep(1)

        if (1):
            return 'outcome1'   # Next: GOTO_DISPLAY
        else:
            return 'outcome2'   # RETURNED_SUCCESS

# ========= SHELF MOTION ========== #
# define state SHELF_OUT
class SHELF_OUT(smach.State):
    def __init__(self):
        smach.State.__init__(self, outcomes=['outcome1', 'outcome2', 'outcome3'])

    def execute(self, userdata):
        print("=== SHELF_OUT ===")

        # goal parameters
        global shelf_num
        global object_id
        global motion_CMD

        global item_order
        global item_count

        global state_result
        global motion_direction_result_flag
        global navi_result_flag

        global ac_motion_direction
        global ac_navi

        global shelf_out_fail_cnt
        global shelfout_recovery_mode

        global skipLine_flag
        global new_display_cnt

        #Reset variable
        skipLine_flag = False
        shelfout_recovery_mode = False
        new_display_cnt = 0

        rate = rospy.Rate(10) # 10hz

        # Set object id and shelf number
        object_id = item_order[item_count]
        shelf_num = 2 - int(object_id/2)


        #debug
        print "=== item_count = ", item_count
        print "=== object_id = ", object_id
        print "=== shelf_num = ", shelf_num

        if(item_count%2 == 1):
            shelf_out_fail_cnt = 0
            return 'outcome1'       # if the shelf if already opened

        # Alignmnet
        use_marker  = 1
        navi_pose_x = 0.0
        navi_pose_y = 0.0
        navi_pose_z = 0.0
        navi_ori_x  = 0.0
        navi_ori_y  = 0.0
        navi_ori_z  = 0.0
        navi_ori_w  = 1.0

        goal_navi = mobile_path_planning.msg.naviGoal(use_marker= use_marker, pose_x=navi_pose_x, pose_y=navi_pose_y, pose_z = navi_pose_z, ori_w = navi_ori_w, ori_x = navi_ori_x, ori_y = navi_ori_y, ori_z = navi_ori_z )
        ac_navi.send_goal(goal_navi)

        # wait in loop until received done flags
        navi_result_flag = False
        while(navi_result_flag is False):
            rate.sleep()

        #Debugging
        print "Robot aligned 95cm away from the shelf"

        motion_cmd = motion_CMD.get('MOTION_SHELFOUT') #3
        goal_motion_direction = wrs_fsm.msg.motion_directionGoal(motion_cmd = motion_cmd, shelf_num = shelf_num, object_id=object_id)
        ac_motion_direction.send_goal(goal_motion_direction)

        # wait in loop until received done flags
        motion_direction_result_flag = False
        while(motion_direction_result_flag is False):
            rate.sleep()

        if(state_result):
            shelf_out_fail_cnt = 0
            return 'outcome1'   # Next: RETURNED_SUCCESS
        elif(shelf_out_fail_cnt == 1):      # Tried 3 times, giving up this shelf
            shelf_out_fail_cnt = 0
            item_count = item_count + 1
            shelfout_recovery_mode = False
            print "shelf out failed. Try shelf in"
            return 'outcome3'   #shelf in
        else:
            shelfout_recovery_mode = True
            shelf_out_fail_cnt = shelf_out_fail_cnt + 1
            return 'outcome3'   # RETURNED_SUCCESS

# define state SHELF_IN
class SHELF_IN(smach.State):
    def __init__(self):
        smach.State.__init__(self, outcomes=['outcome1', 'outcome2'])

    def execute(self, userdata):
        print("=== SHELF_IN ===")

        # goal parameters
        global shelf_num
        global object_id
        global motion_CMD

        global item_count

        global state_result
        global motion_direction_result_flag
        global ac_motion_direction

        global shelfout_recovery_mode
        global skipLine_flag

        rate = rospy.Rate(10) # 10hz

        if(shelfout_recovery_mode == True):
            print "shelf in recovery mode"
            motion_cmd = motion_CMD.get('MOTION_SHELFIN') #4
            goal_motion_direction = wrs_fsm.msg.motion_directionGoal(motion_cmd = motion_cmd, shelf_num = shelf_num, object_id=object_id)
            ac_motion_direction.send_goal(goal_motion_direction)

            # wait in loop until received done flags
            motion_direction_result_flag = False
            while(motion_direction_result_flag is False):
                rate.sleep()

            return 'outcome2'   # Next: SHELF_OUT



        # debug
        print "=== item_count = ", item_count
        print "=== object_id = ", object_id
        print "=== shelf_num = ", shelf_num



        # increase item_cnt when one item is finished
        item_count = item_count + 1

        if(item_count%2 == 1):
            print "return to outcome2"
            return 'outcome2'

        motion_cmd = motion_CMD.get('MOTION_SHELFIN') #4
        goal_motion_direction = wrs_fsm.msg.motion_directionGoal(motion_cmd = motion_cmd, shelf_num = shelf_num, object_id=object_id)
        ac_motion_direction.send_goal(goal_motion_direction)

        # wait in loop until received done flags
        motion_direction_result_flag = False
        while(motion_direction_result_flag is False):
            rate.sleep()

        if (item_count == item_types):
            return 'outcome1'   # Next: GOTO_HOME
        else:
            return 'outcome2'   # SHELF_OUT


# define state DETECT_ONSHELF
class DETECT_ONSHELF(smach.State):
    def __init__(self):
        smach.State.__init__(self, outcomes=['outcome1', 'outcome2'])

    def execute(self, userdata):
        print("=== DETECT_ONSHELF ===")

        # goal parameters
        global shelf_num
        global object_id
        global motion_CMD

        global state_result
        global motion_direction_result_flag
        global ac_motion_direction
        global skipLine_flag

        rate = rospy.Rate(10) # 10hz

        #debug
        print "=== item_count = ", item_count
        print "=== object_id = ", object_id
        print "=== shelf_num = ", shelf_num

        motion_cmd = motion_CMD.get('MOTION_DETECT_ONSHELF') #6 #MOTION_DETECT_ONSHELF
        goal_motion_direction = wrs_fsm.msg.motion_directionGoal(motion_cmd = motion_cmd, shelf_num = shelf_num, object_id=object_id)
        ac_motion_direction.send_goal(goal_motion_direction)

        # wait in loop until received done flags
        motion_direction_result_flag = False
        while(motion_direction_result_flag is False):
            rate.sleep()

        if (1):
            return 'outcome1'   # Next: RETURNED_SUCCESS
        else:
            return 'outcome2'   # RETURNED_SUCCESS


# ========= CLEANING ACTIONS ========== #
# define state DISPOSE
class DISPOSE(smach.State):
    def __init__(self):
        smach.State.__init__(self, outcomes=['outcome1', 'outcome2'])

    def execute(self, userdata):
        print("=== DISPOSE ===")

        # goal parameters
        global shelf_num
        global object_id
        global motion_CMD

        global state_result
        global motion_direction_result_flag
        global ac_motion_direction
        global skipLine_flag

        rate = rospy.Rate(10) # 10hz

        print "skipLine_flag: " + str(skipLine_flag)
        if(skipLine_flag == True):                         #if previous state result is FALLDOWN
            print  "FALLDOWN"
#            # MOTION: HOME POSE
#            motion_cmd = motion_CMD.get('MOTION_HOME_POSE') #10  #MOTION_HOME_POSE
#            goal_motion_direction = wrs_fsm.msg.motion_directionGoal(motion_cmd = motion_cmd, shelf_num = shelf_num, object_id=object_id)
#            ac_motion_direction.send_goal(goal_motion_direction)
            return 'outcome1'                               #jump to next state directly

        #debug
        print "=== item_count = ", item_count
        print "=== object_id = ", object_id
        print "=== shelf_num = ", shelf_num

        if(state_result == False):                          #if previous state result is Fasle = if dipose item is not found
            print  "previous states failed. Skip this state"
            return 'outcome1'                               #jump to next state directly



        motion_cmd = motion_CMD.get('MOTION_DISPOSE') #5
        goal_motion_direction = wrs_fsm.msg.motion_directionGoal(motion_cmd = motion_cmd, shelf_num = shelf_num, object_id=object_id)
        ac_motion_direction.send_goal(goal_motion_direction)

        # wait in loop until received done flags
        motion_direction_result_flag = False
        while(motion_direction_result_flag is False):
            rate.sleep()

        if (state_result):
            return 'outcome1'   # Next: RETURNED_SUCCESS
        else:
            return 'outcome2'   # RETURNED_SUCCESS

# define state ALIGN_OLD
class ALIGN_OLD(smach.State):
    def __init__(self):
        smach.State.__init__(self, outcomes=['outcome1', 'outcome2'])

    def execute(self, userdata):
        print("=== ALIGN_OLD ===")

        # goal parameters
        global shelf_num
        global object_id
        global motion_CMD

        global state_result
        global motion_direction_result_flag
        global ac_motion_direction
        global skipLine_flag

        rate = rospy.Rate(10) # 10hz

        if(skipLine_flag == True):                          #if previous state result is FALLDOWN
            print  "FALLDOWN"
            # MOTION: HOME POSE
#            motion_cmd = motion_CMD.get('MOTION_HOME_POSE') #10  #MOTION_HOME_POSE
#            goal_motion_direction = wrs_fsm.msg.motion_directionGoal(motion_cmd = motion_cmd, shelf_num = shelf_num, object_id=object_id)
#            ac_motion_direction.send_goal(goal_motion_direction)
            return 'outcome1'                               #jump to next state directly

        #debug
        print "=== item_count = ", item_count
        print "=== object_id = ", object_id
        print "=== shelf_num = ", shelf_num

        motion_cmd = motion_CMD.get('MOTION_ALIGN_OLD') #7  #MOTION_ALIGN_OLD
        goal_motion_direction = wrs_fsm.msg.motion_directionGoal(motion_cmd = motion_cmd, shelf_num = shelf_num, object_id=object_id)
        ac_motion_direction.send_goal(goal_motion_direction)

        # wait in loop until received done flags
        motion_direction_result_flag = False
        while(motion_direction_result_flag is False):
            rate.sleep()

        if (1):
            return 'outcome1'   # Next: RETURNED_SUCCESS
        else:
            return 'outcome2'   # RETURNED_SUCCESS

# define state SEARCH_NEW
class SEARCH_NEW(smach.State):
    def __init__(self):
        smach.State.__init__(self, outcomes=['outcome1', 'outcome2'])

    def execute(self, userdata):
        print(" === SEARCH_NEW ===")

        # goal parameters
        global shelf_num
        global object_id
        global motion_CMD

        global new_display_cnt
        global state_result
        global motion_direction_result_flag
        global ac_motion_direction
        global skipLine_flag

        rate = rospy.Rate(10) # 10hz


        if(skipLine_flag == True):                        #if previous state result is FALLDOWN
            print  "FALLDOWN"

            # MOTION: HOME POSE
#            motion_cmd = motion_CMD.get('MOTION_HOME_POSE') #10  #MOTION_HOME_POSE
#            goal_motion_direction = wrs_fsm.msg.motion_directionGoal(motion_cmd = motion_cmd, shelf_num = shelf_num, object_id=object_id)
#            ac_motion_direction.send_goal(goal_motion_direction)

            return 'outcome1'                               #jump to next state directly

        #debug
        print "=== item_count = ", item_count
        print "=== object_id = ", object_id
        print "=== shelf_num = ", shelf_num

        motion_cmd = motion_CMD.get('MOTION_SEARCH_NEW') #8  #MOTION_SEARCH_NEW
        goal_motion_direction = wrs_fsm.msg.motion_directionGoal(motion_cmd = motion_cmd, shelf_num = shelf_num, object_id=object_id, put_new = new_display_cnt)
        ac_motion_direction.send_goal(goal_motion_direction)

        # wait in loop until received done flags
        motion_direction_result_flag = False
        while(motion_direction_result_flag is False):
            rate.sleep()

        if (1):
            return 'outcome1'   # Next: RETURNED_SUCCESS
        else:
            return 'outcome2'   # RETURNED_SUCCESS



# define state DISPLAY_NEW
class DISPLAY_NEW(smach.State):
    def __init__(self):
        smach.State.__init__(self, outcomes=['outcome1', 'outcome2'])

    def execute(self, userdata):
        print("=== DISPLAY_NEW ===")

        # goal parameters
        global shelf_num
        global object_id
        global motion_CMD

        global item_count

        global new_display_cnt
        global state_result
        global motion_direction_result_flag
        global ac_motion_direction
        global skipLine_flag

        rate = rospy.Rate(10) # 10hz


        print "new_display_cnt= " + str(new_display_cnt)

        if(skipLine_flag == True):                          #if previous state result is FALLDOWN
            print  "FALLDOWN"
            # MOTION: HOME POSE
#            motion_cmd = motion_CMD.get('MOTION_HOME_POSE') #10  #MOTION_HOME_POSE
#            goal_motion_direction = wrs_fsm.msg.motion_directionGoal(motion_cmd = motion_cmd, shelf_num = shelf_num, object_id=object_id)
#            ac_motion_direction.send_goal(goal_motion_direction)

            return 'outcome1'                               #jump to next state directly

        #debug
        print "=== item_count = ", item_count
        print "=== object_id = ", object_id
        print "=== shelf_num = ", shelf_num

        if(state_result == False):      #if previous state result is Fasle = if dipose item is not found
            print "previous states failed. Skip this state"
            return 'outcome1'           #jump to next state directly

        motion_cmd = motion_CMD.get('MOTION_DISPLAY_NEW') #9                  #MOTION_DISPLAY_NEW
        goal_motion_direction = wrs_fsm.msg.motion_directionGoal(motion_cmd = motion_cmd, shelf_num = shelf_num, object_id=object_id, put_new = new_display_cnt)
        ac_motion_direction.send_goal(goal_motion_direction)

        # wait in loop until received done flags
        motion_direction_result_flag = False
        while(motion_direction_result_flag is False):
            rate.sleep()

        print "new_display_cnt" + str(new_display_cnt)

        if(object_id == 0 or object_id == 1 or object_id == 5):
            new_display_cnt = new_display_cnt + 1
            print new_display_cnt
            if(new_display_cnt < 2):
                return 'outcome2'
            else:
                new_display_cnt = 0

        if (1):
            return 'outcome1'   # SHELF_IN
        else:
            return 'outcome2'   # SEARCH_NEW



# ==== Base Moving States ====
# define state GOTO_DISPLAY
class GOTO_DISPLAY(smach.State):
    def __init__(self):
        smach.State.__init__(self, outcomes=['outcome1', 'outcome2'])

    def execute(self, userdata):
        print("=== GOTO_DISPLAY ===")

        # goal parameters
        global shelf_num
        global object_id
        global motion_CMD

        global state_result
        global navi_result_flag
        rate = rospy.Rate(10) # 10hz

        # navi
        global ac_navi
        global navi_pose_x, navi_pose_y, navi_pose_z
        global navi_ori_x, navi_ori_y, navi_ori_z, navi_ori_w
        # motion
        global motion_direction_result_flag

        # MOTION: HOME POSE
        motion_cmd = motion_CMD.get('MOTION_HOME_POSE') #10  #MOTION_HOME_POSE
        goal_motion_direction = wrs_fsm.msg.motion_directionGoal(motion_cmd = motion_cmd, shelf_num = shelf_num, object_id=object_id)
        ac_motion_direction.send_goal(goal_motion_direction)

        # wait in loop until received done flags
        motion_direction_result_flag = False
        while(motion_direction_result_flag is False):
            rate.sleep()

        # Point 1
        #Set goal position
        use_marker  = 0
        navi_pose_x = 0.0
        navi_pose_y = -0.95 #-0.85
        navi_pose_z = 0.0
        navi_ori_x  = 0.0
        navi_ori_y  = 0.0
        navi_ori_z  = 0.0
        navi_ori_w  = 1.0

        goal_navi = mobile_path_planning.msg.naviGoal(use_marker= use_marker, pose_x=navi_pose_x, pose_y=navi_pose_y, pose_z = navi_pose_z, ori_w = navi_ori_w, ori_x = navi_ori_x, ori_y = navi_ori_y, ori_z = navi_ori_z )
        ac_navi.send_goal(goal_navi)

        # wait in loop until received done flags
        navi_result_flag = False
        while(navi_result_flag is False):
            rate.sleep()

        #Debugging
        print "at point 1"

        # Point 2
        #Set goal position
        use_marker  = 0
        navi_pose_x = 2.5 #2.6 #2.5
        navi_pose_y = 0.0
        navi_pose_z = 0.0
        navi_ori_x  = 0.0
        navi_ori_y  = 0.0
        navi_ori_z  = 0.0
        navi_ori_w  = 1.0

        goal_navi = mobile_path_planning.msg.naviGoal(use_marker= use_marker, pose_x=navi_pose_x, pose_y=navi_pose_y, pose_z = navi_pose_z, ori_w = navi_ori_w, ori_x = navi_ori_x, ori_y = navi_ori_y, ori_z = navi_ori_z )
        ac_navi.send_goal(goal_navi)

        # wait in loop until received done flags
        navi_result_flag = False
        while(navi_result_flag is False):
            rate.sleep()

        #Debugging
        print "at point 2"

        # Point 3
        #Set goal position
        use_marker  = 0
        navi_pose_x = 0.0
        navi_pose_y = -0.85
        navi_pose_z = 0.0
        navi_ori_x  = 0.0
        navi_ori_y  = 0.0
        navi_ori_z  = 0.0
        navi_ori_w  = 1.0

        goal_navi = mobile_path_planning.msg.naviGoal(use_marker= use_marker, pose_x=navi_pose_x, pose_y=navi_pose_y, pose_z = navi_pose_z, ori_w = navi_ori_w, ori_x = navi_ori_x, ori_y = navi_ori_y, ori_z = navi_ori_z )
        ac_navi.send_goal(goal_navi)

        # wait in loop until received done flags
        navi_result_flag = False
        while(navi_result_flag is False):
            rate.sleep()

        #Debugging
        print "at point 3"

        # Point 4
        #Set goal position
        use_marker  = 0
        navi_pose_x = 0.0
        navi_pose_y = 0.0
        navi_pose_z = 0.0
        navi_ori_x  = 0.0
        navi_ori_y  = 0.0
        navi_ori_z  = 0.707
        navi_ori_w  = 0.707

        goal_navi = mobile_path_planning.msg.naviGoal(use_marker= use_marker, pose_x=navi_pose_x, pose_y=navi_pose_y, pose_z = navi_pose_z, ori_w = navi_ori_w, ori_x = navi_ori_x, ori_y = navi_ori_y, ori_z = navi_ori_z )
        ac_navi.send_goal(goal_navi)

        # wait in loop until received done flags
        navi_result_flag = False
        while(navi_result_flag is False):
            rate.sleep()

        #Debugging
        print "at point 4"

        if(1):
            return 'outcome1'   # Next: SHELF_OUT
        else:
            return 'outcome2'   #


# define state GOTO_HOME
class GOTO_HOME(smach.State):
    def __init__(self):
        smach.State.__init__(self, outcomes=['outcome1', 'outcome2'])

    def execute(self, userdata):
        print("=== GOTO_HOME ===")

        # goal parameters
        global shelf_num
        global object_id
        global motion_CMD

        global state_result
        global navi_result_flag

        global ac_navi
        global navi_pose_x, navi_pose_y, navi_pose_z
        global navi_ori_x, navi_ori_y, navi_ori_z, navi_ori_w

        rate = rospy.Rate(10) # 10hz

        # Point 0
        #Set goal position
        use_marker  = 1
        navi_pose_x = 0.0
        navi_pose_y = 0.0
        navi_pose_z = 0.0
        navi_ori_x  = 0.0
        navi_ori_y  = 0.0
        navi_ori_z  = 0.0
        navi_ori_w  = 1.0

        goal_navi = mobile_path_planning.msg.naviGoal(use_marker= use_marker, pose_x=navi_pose_x, pose_y=navi_pose_y, pose_z = navi_pose_z, ori_w = navi_ori_w, ori_x = navi_ori_x, ori_y = navi_ori_y, ori_z = navi_ori_z )
        ac_navi.send_goal(goal_navi)

        # wait in loop until received done flags
        navi_result_flag = False
        while(navi_result_flag is False):
            rate.sleep()

        #Debugging
        print "at point 0"


        # Point 1
        #Set goal position
        use_marker  = 0
        navi_pose_x = 0.0
        navi_pose_y = 0.0
        navi_pose_z = 0.0
        navi_ori_x  = 0.0
        navi_ori_y  = 0.0
        navi_ori_z  = 0.707
        navi_ori_w  = 0.707

        goal_navi = mobile_path_planning.msg.naviGoal(use_marker= use_marker, pose_x=navi_pose_x, pose_y=navi_pose_y, pose_z = navi_pose_z, ori_w = navi_ori_w, ori_x = navi_ori_x, ori_y = navi_ori_y, ori_z = navi_ori_z )
        ac_navi.send_goal(goal_navi)

        # wait in loop until received done flags
        navi_result_flag = False
        while(navi_result_flag is False):
            rate.sleep()

        #Debugging
        print "at point 1"

        # Point 2
        #Set goal position
        use_marker  = 0
        navi_pose_x = 0.0
        navi_pose_y = -0.85
        navi_pose_z = 0.0
        navi_ori_x  = 0.0
        navi_ori_y  = 0.0
        navi_ori_z  = 0.0
        navi_ori_w  = 1.0

        goal_navi = mobile_path_planning.msg.naviGoal(use_marker= use_marker, pose_x=navi_pose_x, pose_y=navi_pose_y, pose_z = navi_pose_z, ori_w = navi_ori_w, ori_x = navi_ori_x, ori_y = navi_ori_y, ori_z = navi_ori_z )
        ac_navi.send_goal(goal_navi)

        # wait in loop until received done flags
        navi_result_flag = False
        while(navi_result_flag is False):
            rate.sleep()

        #Debugging
        print "at point 2"

        # Point 3
        #Set goal position
        use_marker  = 0
        navi_pose_x = 2.4 #2.5
        navi_pose_y = -0.1
        navi_pose_z = 0.0
        navi_ori_x  = 0.0
        navi_ori_y  = 0.0
        navi_ori_z  = 0.0
        navi_ori_w  = 1.0

        goal_navi = mobile_path_planning.msg.naviGoal(use_marker= use_marker, pose_x=navi_pose_x, pose_y=navi_pose_y, pose_z = navi_pose_z, ori_w = navi_ori_w, ori_x = navi_ori_x, ori_y = navi_ori_y, ori_z = navi_ori_z )
        ac_navi.send_goal(goal_navi)

        # wait in loop until received done flags
        navi_result_flag = False
        while(navi_result_flag is False):
            rate.sleep()

        #Debugging
        print "at point 3"

        # Point 4
        #Set goal position
        use_marker  = 0
        navi_pose_x = 0.0
        navi_pose_y = -1.5 #0.85
        navi_pose_z = 0.0
        navi_ori_x  = 0.0
        navi_ori_y  = 0.0
        navi_ori_z  = 0.0
        navi_ori_w  = 1.0

        goal_navi = mobile_path_planning.msg.naviGoal(use_marker= use_marker, pose_x=navi_pose_x, pose_y=navi_pose_y, pose_z = navi_pose_z, ori_w = navi_ori_w, ori_x = navi_ori_x, ori_y = navi_ori_y, ori_z = navi_ori_z )
        ac_navi.send_goal(goal_navi)

        # wait in loop until received done flags
        navi_result_flag = False
        while(navi_result_flag is False):
            rate.sleep()

        #Debugging
        print "at point 4"

#        # Point 5
#        #Set goal position
#        use_marker  = 0
#        navi_pose_x = 0.0
#        navi_pose_y = 0.0
#        navi_pose_z = 0.0
#        navi_ori_x  = 0.0
#        navi_ori_y  = 0.0
#        navi_ori_z  = -1.0
#        navi_ori_w  = 0.0

#        goal_navi = mobile_path_planning.msg.naviGoal(use_marker= use_marker, pose_x=navi_pose_x, pose_y=navi_pose_y, pose_z = navi_pose_z, ori_w = navi_ori_w, ori_x = navi_ori_x, ori_y = navi_ori_y, ori_z = navi_ori_z )
#        ac_navi.send_goal(goal_navi)

        # wait in loop until received done flags
        navi_result_flag = False
        while(navi_result_flag is False):
            rate.sleep()

        #Debugging
        print "at point 5"

        if(1):
            return 'outcome1'   # Next: SHELF_OUT
        else:
            return 'outcome2'   #


# ================================================ main  ================================================ #
def main():
    #initialize node
    rospy.init_node('RB5_test1')


    #action variables
    global ac_motion_direction
    global ac_navi

    # Creates the SimpleActionClient, passing the type of the action to the constructor.
    ac_navi = actionlib.SimpleActionClient('hubo_navigation', mobile_path_planning.msg.naviAction)
    ac_motion_direction = actionlib.SimpleActionClient('motion_direction', wrs_fsm.msg.motion_directionAction)

    # Waits until the action server has started up and started
    # listening for goals.    
    rospy.loginfo("Wait for server starts")
    ac_motion_direction.wait_for_server()
    ac_navi.wait_for_server()

    # Subscribers
    rospy.Subscriber("/hubo_navigation/result", naviActionResult, navi_result_callback, queue_size=1, buff_size=10)
    rospy.Subscriber("/motion_direction/result", motion_directionActionResult, motion_direction_result_callback, queue_size=1, buff_size=10)


    # Create a SMACH state machine
    sm = smach.StateMachine(outcomes=['ABORTED', 'RETURNED_SUCCESS'])

    # Open the container
    with sm:

        # Add states to the container
        smach.StateMachine.add('IDLE', IDLE(),
                               transitions={'outcome1': 'GOTO_DISPLAY', 'outcome2':'RETURNED_SUCCESS'})
        smach.StateMachine.add('GOTO_DISPLAY', GOTO_DISPLAY(),
                               transitions={'outcome1': 'SHELF_OUT', 'outcome2':'ABORTED'})
        smach.StateMachine.add('SHELF_OUT', SHELF_OUT(),
                               transitions={'outcome1': 'DETECT_ONSHELF', 'outcome2':'SHELF_OUT', 'outcome3':'SHELF_IN'})
        smach.StateMachine.add('DETECT_ONSHELF', DETECT_ONSHELF(),
                               transitions={'outcome1': 'DISPOSE', 'outcome2':'DISPOSE'})
        smach.StateMachine.add('DISPOSE', DISPOSE(),
                               transitions={'outcome1': 'ALIGN_OLD', 'outcome2':'ALIGN_OLD'})
        smach.StateMachine.add('ALIGN_OLD', ALIGN_OLD(),
                               transitions={'outcome1': 'SEARCH_NEW', 'outcome2':'SEARCH_NEW'})
        smach.StateMachine.add('SEARCH_NEW', SEARCH_NEW(),
                               transitions={'outcome1': 'DISPLAY_NEW', 'outcome2':'DISPLAY_NEW'})
        smach.StateMachine.add('DISPLAY_NEW', DISPLAY_NEW(),
                               transitions={'outcome1': 'SHELF_IN', 'outcome2':'SEARCH_NEW'})
        smach.StateMachine.add('SHELF_IN', SHELF_IN(),
                               transitions={'outcome1': 'GOTO_HOME', 'outcome2':'SHELF_OUT'})
        smach.StateMachine.add('GOTO_HOME', GOTO_HOME(),
                               transitions={'outcome1': 'RETURNED_SUCCESS', 'outcome2':'ABORTED'})

        # SHELF_OUT, SHELF_IN, DISPOSE, DETECT_ONSHELF, GOTO_DISPLAY, GOTO_DISPLAY, GOTO_HOME, ALIGN_OLD, SEARCH_NEW, DISPLAY_NEW


        # state Machine Test for Navi module
#        smach.StateMachine.add('IDLE', IDLE(),
#                               transitions={'outcome1': 'GOTO_DISPLAY', 'outcome2':'RETURNED_SUCCESS'})
#        smach.StateMachine.add('GOTO_DISPLAY', GOTO_DISPLAY(),
#                               transitions={'outcome1': 'GOTO_HOME', 'outcome2':'RETURNED_SUCCESS'})
#        smach.StateMachine.add('GOTO_HOME', GOTO_HOME(),
#                               transitions={'outcome1': 'RETURNED_SUCCESS', 'outcome2':'RETURNED_SUCCESS'})


#motion State

#        smach.StateMachine.add('IDLE', IDLE(),
#                               transitions={'outcome1': 'SHELF_OUT', 'outcome2':'RETURNED_SUCCESS'})
##        smach.StateMachine.add('GOTO_DISPLAY', GOTO_DISPLAY(),
##                               transitions={'outcome1': 'SHELF_OUT', 'outcome2':'ABORTED'})
#        smach.StateMachine.add('SHELF_OUT', SHELF_OUT(),
#                               transitions={'outcome1': 'DETECT_ONSHELF', 'outcome2':'SHELF_OUT', 'outcome3':'SHELF_IN'})
#        smach.StateMachine.add('DETECT_ONSHELF', DETECT_ONSHELF(),
#                               transitions={'outcome1': 'DISPOSE', 'outcome2':'DISPOSE'})
#        smach.StateMachine.add('DISPOSE', DISPOSE(),
#                               transitions={'outcome1': 'ALIGN_OLD', 'outcome2':'ALIGN_OLD'})
#        smach.StateMachine.add('ALIGN_OLD', ALIGN_OLD(),
#                               transitions={'outcome1': 'SEARCH_NEW', 'outcome2':'SEARCH_NEW'})
#        smach.StateMachine.add('SEARCH_NEW', SEARCH_NEW(),
#                               transitions={'outcome1': 'DISPLAY_NEW', 'outcome2':'DISPLAY_NEW'})
#        smach.StateMachine.add('DISPLAY_NEW', DISPLAY_NEW(),
#                               transitions={'outcome1': 'SHELF_IN', 'outcome2':'SEARCH_NEW'})
#        smach.StateMachine.add('SHELF_IN', SHELF_IN(),
#                               transitions={'outcome1': 'RETURNED_SUCCESS', 'outcome2':'SHELF_OUT'})
##        smach.StateMachine.add('GOTO_HOME', GOTO_HOME(),
##                               transitions={'outcome1': 'RETURNED_SUCCESS', 'outcome2':'ABORTED'})

    # Create and start the introspection server (for smach viewer)
    sis = smach_ros.IntrospectionServer('server_name', sm, '/SM_ROOT/RB5_test1')
    sis.start()


    # Execute SMACH plan
    outcome = sm.execute()

    # Wait for ctrl-c to stop the application
    rospy.spin()
    sis.stop()

if __name__ == '__main__':
    main()
