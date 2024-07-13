import sys
sys.path.append('../Resources')
import traceback
import Resources
import common
import images

##### START #####
TOTAL = 1
Resources.start_set('Notifications')
Resources.start_cadex_app()

##### TEST CASES #####
### Check notifications ###
def check_notifications():
    test_name = 'CHECK NOTIFICATIONS'
    Resources.announce_text(test_name+Resources.test_num(1, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.import_model('samples', images.model_samples_asm_torsen_differential, common.MODEL_ATD, common.FORMAT_STP)

    ### ### ### ### ### ###
    click(images.notifications_icon)
    wait(1)
    click(Pattern(images.close_button_small).similar(0.95))
    click(images.clear_all_button)
    ### ### ### ### ### ###
    
    return Resources.special_condition(False, False, '')
    
### Execute test cases
try:
    passed1 = True
    passed1 = Resources.result(check_notifications())
    ##### TEST CASES END #####
    
    ##### END #####
    Resources.close_cadex_app()
    Resources.finish_set(passed1)

except:
    tb = traceback.format_exc()
    Resources.except_set(tb)
