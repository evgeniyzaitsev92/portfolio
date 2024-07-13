import sys
sys.path.append('../Resources')
import traceback
import Resources
import common
import images

##### START #####
TOTAL = 5
Resources.start_set('Representations')
Resources.start_cadex_app()

#Resources.import_model('samples', images.model_samples_asm_torsen_differential, common.MODEL_ATD, common.FORMAT_STP)

##### TEST CASES #####
### Create and delete representation ###
def create_and_delete_representation():
    test_name = 'CREATE AND DELETE REPRESENTATIONS'
    Resources.announce_text(test_name+Resources.test_num(1, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.import_model('samples', images.model_samples_asm_torsen_differential, common.MODEL_ATD, common.FORMAT_STP)

    ### ### ### ### ### ###
    click(images.repres_icon)
    click(images.repres_poly1)
    Resources.display_complete()
    click(images.repres_mesh)
    wait(images.repres_mesh_add, 10)
    click(images.repres_mesh_add)
    Resources.display_complete()
    for _ in range(3):
        wait(1)
        click(Pattern(images.repres_options_blue).similar(0.95))
        wait(images.repres_options_blue_delete, 10)
        click(images.repres_options_blue_delete)
    click(images.close_pinned_panel)
    Resources.back_to_start_page()
    ### ### ### ### ### ###
    
    return Resources.special_condition(False, False, '')

### New representation with applied explode ###
def new_representation_with_applied_explode():
    test_name = 'NEW REPRESENTATION WITH APPLIED EXPLODE'
    Resources.announce_text(test_name+Resources.test_num(2, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.import_model('samples', images.model_samples_asm_torsen_differential, common.MODEL_ATD, common.FORMAT_STP)

    ### ### ### ### ### ###
    mouseMove(images.explode)
    click(images.explode_edit)
    type('6')
    type(Key.ENTER)
    hSC, passedExplode, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep_Explode', common.FORMAT_STP, test_name+' BREP EXPLODE', 'Explode: ')
    
    click(images.repres_icon)
    click(images.repres_mesh)
    wait(images.repres_mesh_add, 10)
    click(images.repres_mesh_add)
    Resources.display_complete()
    hSC, passedExplodeResetPoly, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_Poly_New', common.FORMAT_STP, test_name+' POLY EXPLODE RESET', 'Explode is reset in Poly: ')
    
    click(Pattern(images.repres_options_blue).similar(0.95))
    wait(images.repres_options_blue_delete, 10)
    click(images.repres_options_blue_delete)
    click(images.close_pinned_panel)
    hSC, passedExplodeResetBRep, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep', common.FORMAT_STP, test_name+' BREP EXPLODE RESET', 'Explode is reset in BRep: ')
    
    passed, mes = Resources.special_condition_validate('image', passedExplode, passedExplodeResetPoly, passedExplodeResetBRep)
    Resources.back_to_start_page()
    ### ### ### ### ### ###
    
    return Resources.special_condition(True, passed, mes)

### New representation with hidden items ###
def new_representation_with_hidden_items():
    test_name = 'NEW REPRESENTATION WITH HIDDEN ITEMS'
    Resources.announce_text(test_name+Resources.test_num(3, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.import_model('samples', images.model_samples_asm_torsen_differential, common.MODEL_ATD, common.FORMAT_STP)

    ### ### ### ### ### ###
    click(images.structure_icon)
    doubleClick(images.structure_asm_torsen_differential_stp)
    doubleClick(Pattern(images.structure_asm_torsen_differential).similar(0.95))
    click(Pattern(images.structure_asm_torsen_differential_torsen_bevel_gear_set).similar(0.95))
    rightClick(Pattern(images.structure_asm_torsen_differential_torsen_bevel_gear_set).similar(0.95))
    click(images.element_hide)
    click(images.close_pinned_panel)
    hSC, passedHidden, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep_Hidden2', common.FORMAT_STP, test_name+' BREP HIDDEN', 'Hidden in BRep: ')
    
    click(images.repres_icon)
    click(images.repres_mesh)
    wait(images.repres_mesh_add, 10)
    click(images.repres_mesh_add)
    Resources.display_complete()
    hSC, passedHiddenPoly, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_Poly_New_Hidden', common.FORMAT_STP, test_name+' POLY NEW HIDDEN', 'Hidden in Poly: ')
    
    click(Pattern(images.repres_options_blue).similar(0.95))
    wait(images.repres_options_blue_delete, 10)
    click(images.repres_options_blue_delete)
    click(images.close_pinned_panel)
    hSC, passedHidden2, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep_Hidden2', common.FORMAT_STP, test_name+' BREP HIDDEN AFTER REPR DEL', 'Hidden in BRep after repr is deleted: ')
    
    passed, mes = Resources.special_condition_validate('image', passedHidden, passedHiddenPoly, passedHidden2)
    Resources.back_to_start_page()
    ### ### ### ### ### ###
    
    return Resources.special_condition(True, passed, mes)

### New representation with applied sectioning ###
def new_representation_with_applied_sectioning():
    test_name = 'NEW REPRESENTATION WITH APPLIED SECTIONING'
    Resources.announce_text(test_name+Resources.test_num(4, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.import_model('samples', images.model_samples_asm_torsen_differential, common.MODEL_ATD, common.FORMAT_STP)

    ### ### ### ### ### ###
    click(images.sectioning_icon)
    click(images.sectioning_add_plane)
    click(images.close_pinned_panel)
    hSC, passedBRepSectioning, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep_Sectioning', common.FORMAT_STP, test_name+' BREP SECTIONING', 'Sectioning in BRep: ')
    
    click(images.repres_icon)
    click(images.repres_mesh)
    wait(images.repres_mesh_add, 10)
    click(images.repres_mesh_add)
    Resources.display_complete()
    hSC, passedPolySectioning, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_Poly_New_Sectioning', common.FORMAT_STP, test_name+' POLY NEW SECTIONING', 'Sectioning in Poly: ')
    
    click(Pattern(images.repres_options_blue).similar(0.95))
    wait(images.repres_options_blue_delete, 10)
    click(images.repres_options_blue_delete)
    click(images.close_pinned_panel)
    hSC, passedBRepSectioning2, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep_Sectioning', common.FORMAT_STP, test_name+' BREP SECTIONING AFTER REPR DEL', 'Sectioning in BRep after repr is deleted: ')
    
    passed, mes = Resources.special_condition_validate('image', passedBRepSectioning, passedPolySectioning, passedBRepSectioning2)
    Resources.back_to_start_page()
    ### ### ### ### ### ###
    
    return Resources.special_condition(True, passed, mes)

### New representation with transformation changes ### NOT FINISHED!
def new_representation_with_transformation_change():
    test_name = 'NEW REPRESENTATION WITH TRANSFORMATION CHANGE'
    Resources.announce_text(test_name+Resources.test_num(5, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.import_model('samples', images.model_samples_asm_torsen_differential, common.MODEL_ATD, common.FORMAT_STP)

    ### ### ### ### ### ###
    click(images.manipulator)
    click(Pattern(images.manipulator_element_for_move).similar(0.95))
    click(Pattern(images.manipulator_arrow).similar(0.95))
    dragDrop(Pattern(images.manipulator_arrow_hover).similar(0.95), Pattern(images.properties_icon).similar(0.95))
    click(images.apply_button)
    
    ### ### ### ### ### ###
    
    return Resources.special_condition(True, passed, mes)
    
### Execute test cases
try:
    passed1 = passed2 = passed3 = passed4 = True
    passed1 = Resources.result(create_and_delete_representation())
    passed2 = Resources.result(new_representation_with_applied_explode())
    passed3 = Resources.result(new_representation_with_hidden_items())
    passed4 = Resources.result(new_representation_with_applied_sectioning())
    #Resources.result(new_representation_with_transformation_change())
    ##### TEST CASES END #####

    ##### END #####
    Resources.close_cadex_app()
    Resources.finish_set(passed1, passed2, passed3, passed4)

except:
    tb = traceback.format_exc()
    Resources.except_set(tb)
