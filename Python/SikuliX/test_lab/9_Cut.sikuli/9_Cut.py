import sys
sys.path.append('../Resources')
import traceback
import Resources
import common
import images

##### START #####
TOTAL = 2
Resources.start_set('Cut')
Resources.start_cadex_app()

##### TEST CASES #####
### Section planes control ###
def section_planes_control():
    test_name = 'SECTION PLANES CONTROL'
    Resources.announce_text(test_name+Resources.test_num(1, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.import_model('samples', images.model_samples_asm_torsen_differential, common.MODEL_ATD, common.FORMAT_STP)
    
    ### ### ### ### ### ###
    click(images.sectioning_icon)
    click(images.sectioning_add_plane)
    click(images.sectioning_plane_y)
    click(images.sectioning_plane_z)
    click(images.sectioning_plane_x)
    click(images.switch2_off)
    click(images.sectioning_edit)
    for _ in range(5): type(Key.BACKSPACE)
    paste('-50')
    type(Key.ENTER)
    wait(1)
    click(images.sectioning_add_plane)
    wait(1)
    click(images.sectioning_add_plane)
    hSC, passedCutXYZ, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep_Cut_XYZ', common.FORMAT_STP, test_name+' CUT XYZ', 'Cut XYZ: ')

    click(Pattern(images.close_button_small).similar(0.95))
    click(images.close_pinned_panel)
    hSC, passedCutYZ, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep_Cut_YZ', common.FORMAT_STP, test_name+' CUT YZ', 'Cut YZ: ')

    passed, mes = Resources.special_condition_validate('image', passedCutXYZ, passedCutYZ)
    Resources.back_to_start_page()
    ### ### ### ### ### ###

    return Resources.special_condition(True, passed, mes)

### Sectioning with activated explode ###
def sectioning_with_activated_explode():
    test_name = 'SECTIONING WITH ACTIVATED EXPLODE'
    Resources.announce_text(test_name+Resources.test_num(2, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.import_model('samples', images.model_samples_asm_torsen_differential, common.MODEL_ATD, common.FORMAT_STP)
    
    ### ### ### ### ### ###
    click(images.sectioning_icon)
    click(images.sectioning_add_plane)
    mouseMove(images.explode)
    click(images.explode_edit)
    type('6')
    type(Key.ENTER)
    hSC, passedCutExplode, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep_Cut_Explode', common.FORMAT_STP, test_name+' CUT EXPLODE', 'Cut Explode: ')

    click(Pattern(images.close_button_small).similar(0.95))
    click(images.close_pinned_panel)
    hSC, passedNoCutExplode, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep_Explode', common.FORMAT_STP, test_name+' NO CUT EXPLODE', 'No Cut Explode: ')

    passed, mes = Resources.special_condition_validate('image', passedCutExplode, passedNoCutExplode)
    ### ### ### ### ### ###

    return Resources.special_condition(True, passed, mes)
    
### Execute test cases
try:
    passed1 = passed2 = True
    passed1 = Resources.result(section_planes_control())
    passed2 = Resources.result(sectioning_with_activated_explode())
    ##### TEST CASES END #####
    
    ##### END #####
    Resources.close_cadex_app()
    Resources.finish_set(passed1, passed2)

except:
    tb = traceback.format_exc()
    Resources.except_set(tb)
