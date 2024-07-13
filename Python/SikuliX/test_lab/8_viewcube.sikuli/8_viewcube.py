import sys
sys.path.append('../Resources')
import traceback
import Resources
import common
import images

##### START #####
TOTAL = 1
Resources.start_set('Viewcube')
Resources.start_cadex_app()

##### TEST CASES #####
### Rotate model using Viewcube ###
def rotate_model_using_viewcube():
    test_name = 'ROTATE MODEL USING VIEWCUBE'
    Resources.announce_text(test_name+Resources.test_num(1, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.import_model('samples', images.model_samples_asm_torsen_differential, common.MODEL_ATD, common.FORMAT_STP)
    
    ### ### ### ### ### ###
    click(images.viewcube_left)
    click(images.viewcube_left_down)
    click(images.viewcube_up)
    click(images.viewcube_right_down)
    click(images.viewcube_right)
    click(images.viewcube_down)
    click(images.viewcube_left)
    click(images.viewcube_corner)
    click(images.viewcube_face_right)

    result = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep_Viewcube', common.FORMAT_STP, test_name+' VIEWCUBE', '')
    ### ### ### ### ### ###
    
    return result
    
### Execute test cases
try:
    passed1 = True
    passed1 = Resources.result(rotate_model_using_viewcube())
    ##### TEST CASES END #####
    
    ##### END #####
    Resources.close_cadex_app()
    Resources.finish_set(passed1)

except:
    tb = traceback.format_exc()
    Resources.except_set(tb)
