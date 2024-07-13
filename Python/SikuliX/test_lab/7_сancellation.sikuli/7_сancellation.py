import sys
sys.path.append('../Resources')
import traceback
import Resources
import common
import images

##### START #####
TOTAL = 2
Resources.start_set('Cancellation')
Resources.start_cadex_app()

##### TEST CASES #####
### Cancel import ###
def cancel_import():
    test_name = 'CANCEL IMPORT'
    Resources.announce_text(test_name+Resources.test_num(1, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    
    ### ### ### ### ### ###
    wait(images.new_file, 10)
    click(images.new_file)
    wait(1)
    paste(common.CADEX_SERVER+'TestModels\\JT\\sener\\RL_Foran_Scene-Big.jt')
    type(Key.ENTER)
    click(Pattern(images.cancel_notification).similar(0.95))
    wait(Pattern(images.import_canceled).similar(0.95), FOREVER)
    Resources.back_to_start_page()
    ### ### ### ### ### ###
    
    return Resources.special_condition(False, False, '')

### Cancel displaying ###
def cancel_displaying():
    test_name = 'CANCEL DISPLAYING'
    Resources.announce_text(test_name+Resources.test_num(2, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    
    ### ### ### ### ### ###
    wait(images.new_file, 10)
    click(images.new_file)
    wait(1)
    paste(common.CADEX_SERVER+'TestModels\\JT\\sener\\RL_Foran_Scene-Big.jt')
    type(Key.ENTER)
    wait(Pattern(images.displaying).similar(0.95), FOREVER)
    click(Pattern(images.cancel_notification).similar(0.95))
    wait(Pattern(images.display_canceled).similar(0.95), FOREVER)
    ### ### ### ### ### ###
    
    return Resources.special_condition(False, False, '')
    
### Execute test cases
try:
    passed1 = passed2 = True
    passed1 = Resources.result(cancel_import())
    passed2 = Resources.result(cancel_displaying())
    ##### TEST CASES END #####
    
    ##### END #####
    Resources.close_cadex_app()
    Resources.finish_set(passed1, passed2)

except:
    tb = traceback.format_exc()
    Resources.except_set(tb)
