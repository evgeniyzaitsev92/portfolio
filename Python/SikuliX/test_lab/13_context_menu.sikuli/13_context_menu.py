import sys
sys.path.append('../Resources')
import traceback
import Resources
import common
import images

##### START #####
TOTAL = 5
Resources.start_set('Context menu')
Resources.start_cadex_app()

##### TEST CASES #####            
### Display mode ###
def check_context_menu():
    test_name = 'CHECK CONTEXT MENU'
    Resources.announce_text(test_name+Resources.test_num(1, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.import_model('samples', images.model_samples_asm_torsen_differential, common.MODEL_ATD, common.FORMAT_STP)
        
    ### ### ### ### ### ###
    click(images.structure_icon)
    doubleClick(images.structure_asm_torsen_differential_stp)
    doubleClick(Pattern(images.structure_asm_torsen_differential).similar(0.95))

    # Hide
    click(Pattern(images.structure_asm_torsen_differential_torsen_differential_v3).similar(0.95))
    rightClick(images.structure_asm_torsen_differential_torsen_differential_v3)
    click(images.element_hide)
    hSC, passedBRepHide, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep_Hidden', common.FORMAT_STP, test_name+' HIDE', 'Hide: ')

    # Hide + Hide ghosts
    click(Pattern(images.structure_asm_torsen_differential_torsen_differential_v3).similar(0.95))
    rightClick(images.structure_asm_torsen_differential_torsen_differential_v3)
    click(images.element_hide_ghosts)
    hSC, passedBRepHideHideGhosts, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep_Hide_Hide_Ghosts', common.FORMAT_STP, test_name+' HIDE GHOSTS', 'Hide + Hide Ghosts: ')
    
    # Show only
    click(Pattern(images.structure_asm_torsen_differential_torsen_differential_v3).similar(0.95))
    rightClick(images.structure_asm_torsen_differential_torsen_differential_v3)
    click(images.element_show_only)
    hSC, passedBRepShowOnly, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep_Show_Only', common.FORMAT_STP, test_name+' SHOW ONLY', 'Show only: ')

    # Show all
    click(Pattern(images.structure_asm_torsen_differential_torsen_differential_v3).similar(0.95))
    rightClick(images.structure_asm_torsen_differential_torsen_differential_v3)
    click(images.element_show_all)
    hSC, passedBRepShowAll, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep', common.FORMAT_STP, test_name+' SHOW ALL', 'Show all: ')

    # Clear selection
    click(Pattern(images.structure_asm_torsen_differential_torsen_differential_v3).similar(0.95))
    
    passedBRepCheckSelected = False
    if exists(Pattern(images.model_asm_torsen_differential_selected_body).similar(0.95)):
        passedBRepCheckSelected = True

    rightClick(images.structure_asm_torsen_differential_torsen_differential_v3)
    click(images.element_clear_selection)
    
    passedBRepCheckUnselected = False
    if exists(Pattern(images.model_asm_torsen_differential_unselected_body).similar(0.95)):
        passedBRepCheckUnselected = True

    passedBRepClearSelection = False
    if passedBRepCheckSelected == True and passedBRepCheckUnselected == True:
        passedBRepClearSelection = True
        Resources.html_report_add_comment('ok', 'Clear selection works fine.')
    else:
        Resources.html_report_add_comment('fail', 'Clear selection does not work fine!')
    
    # Manipulate
    click(Pattern(images.structure_asm_torsen_differential_torsen_differential_v3).similar(0.95))
    rightClick(images.structure_asm_torsen_differential_torsen_differential_v3)
    click(images.element_manipulate)
    hSC, passedBRepManipulate, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep_Manipulate', common.FORMAT_STP, test_name+' MANIPULATE', 'Manipulate: ')
    
    ## Result
    Resources.back_to_start_page()
    passed, mes = Resources.special_condition_validate('image', passedBRepHide, passedBRepHideHideGhosts,
            passedBRepShowOnly, passedBRepShowAll, passedBRepClearSelection, passedBRepManipulate)
    ### ### ### ### ### ###

    return Resources.special_condition(True, passed, mes)
    
### Execute test cases
try:
    passed1 = True
    passed1 = Resources.result(check_context_menu())
    ##### TEST CASES END #####
    
    ##### END #####
    Resources.close_cadex_app()
    Resources.finish_set(passed1)

except:
    tb = traceback.format_exc()
    Resources.except_set(tb)
