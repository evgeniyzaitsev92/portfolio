import sys
sys.path.append('../Resources')
import traceback
import Resources
import common
import images

##### START #####
TOTAL = 7
Resources.start_set('Properties')
Resources.start_cadex_app()

Resources.import_model('samples', images.model_samples_asm_torsen_differential, common.MODEL_ATD, common.FORMAT_STP)

##### TEST CASES #####
### Сhanging units ###
def changing_units():
    test_name = 'CHANGING UNITS'
    Resources.announce_text(test_name+Resources.test_num(1, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)

    ### ### ### ### ### ###
    click(images.structure_icon)
    click(images.structure_asm_torsen_differential_stp)
    click(images.properties_icon)

    click(Pattern(images.properties_units_milli).similar(0.95))
    units = [images.properties_units_centi, images.properties_units_meters, images.properties_units_inches,
            images.properties_units_feet, images.properties_units_yards]

    for unit in units:
        click(Pattern(unit).similar(0.95))
        click(Pattern(unit).similar(0.95))
    click(images.properties_units_milli_blue)
    ### ### ### ### ### ###
    
    return Resources.special_condition(False, False, '')

### Сhanging name ###
def changing_name():
    test_name = 'CHANGING NAME'
    Resources.announce_text(test_name+Resources.test_num(2, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)

    ### ### ### ### ### ###
    doubleClick(images.structure_asm_torsen_differential_stp)
    doubleClick(Pattern(images.structure_asm_torsen_differential).similar(0.95))
    click(Pattern(images.structure_asm_torsen_differential_torsen_differential_v3).similar(0.95))
    click(images.properties_edit_name)
    for _ in range(35): type(Key.BACKSPACE)
    paste('new_name')
    type(Key.ENTER)
    ### ### ### ### ### ###
    
    return Resources.special_condition(False, False, '')

### Сhanging color ###
def changing_color():
    test_name = 'CHANGING COLOR'
    Resources.announce_text(test_name+Resources.test_num(3, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)

    ### ### ### ### ### ###
    click(images.properties_color)
    click(images.properties_color_html)
    for _ in range(8): type(Key.BACKSPACE)
    paste('#F08080')
    type(Key.ENTER)
    wait(1)
    type(Key.ESC)
    wait(1)
    click(images.close_pinned_panel)
    
    result = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep_Color', common.FORMAT_STP, test_name, '')
    ### ### ### ### ### ###
    
    return result

### Сhanging opacity ###
def changing_opacity():
    test_name = 'CHANGING OPACITY'
    Resources.announce_text(test_name+Resources.test_num(4, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)

    ### ### ### ### ### ###
    click(images.properties_icon)
    wait(images.properties_opacity_100, 3)
    doubleClick(images.properties_opacity_100)
    paste('49')
    type(Key.ENTER)
    wait(1)
    type(Key.ESC)
    
    result = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep_Color_Opacity', common.FORMAT_STP, test_name, '')
    ### ### ### ### ### ###
    
    return result

### Display bounding box ###
def display_bounding_box():
    test_name = 'DISPLAY BOUNDING BOX'
    Resources.announce_text(test_name+Resources.test_num(5, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)

    ### ### ### ### ### ###
    click(images.properties_icon)
    wait(Pattern(images.switch2_off).similar(0.95), 3)
    click(Pattern(images.switch2_off).similar(0.95))
    result = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep_Color_Opacity_BB', common.FORMAT_STP, test_name, '')
    click(images.switch2_on)
    ### ### ### ### ### ###
    
    return result

### Visualization and Validation ###
def visualization_and_validation():
    test_name = 'VISUALIZATION AND VALIDATION'
    Resources.announce_text(test_name+Resources.test_num(6, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)

    ### ### ### ### ### ###
    doubleClick(images.properties_val)
    click(images.properties_val_volume)
    click(images.properties_val_surface)
    click(images.properties_val_mass)
    doubleClick(images.properties_vis)
    click(images.properties_vis_triangles)
    type(Key.ESC)
    Resources.back_to_start_page()
    ### ### ### ### ### ###
    
    return Resources.special_condition(False, False, '')

### Custom properties ###
def custom_properties():
    test_name = 'CUSTOM PROPERTIES'
    Resources.announce_text(test_name+Resources.test_num(7, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.import_model('new file', common.CADEX_SERVER+'TestModels\\JT\\glovius\\www.planetpdf.com\\9.5\\', '10000_Assembly_Loft_And_Lie_Machine_NoName', 'jt')

    ### ### ### ### ### ###
    click(images.structure_icon)
    click(images.structure_10000_assembly_loft_and_lie_machine_noname_jt)
    click(images.properties_icon)
    wait(1)
    passedCustomHidden = False
    if not exists(images.properties_custom, 10):
        passedCustomHidden = True
        Resources.html_report_add_comment('ok', 'Entire model: "Custom" is hidden.')
    else:
        passedCustomHidden = False
        Resources.html_report_add_comment('fail', 'Entire model: "Custom" is not obviously hidden!')
    
    doubleClick(images.structure_10000_assembly_loft_and_lie_machine_noname_jt)
    click(Pattern(images.structure_10000_assembly_loft_and_lie_machine_noname).similar(0.95))
    passedCustomVisible = False
    if exists(images.properties_custom, 10):
        doubleClick(images.properties_custom)
        passedCustomVisible = True
        Resources.html_report_add_comment('ok', 'Assembly: "Custom" is seen.')
    else:
        passedCustomVisible = False
        Resources.html_report_add_comment('fail', 'Assembly: "Custom" is not obviously seen!')

    passed = False
    mes = ''
    if passedCustomHidden == True and passedCustomVisible == True:
        passed = True
    else:
        passed = False
        mes = 'Something went wrong!'
    ### ### ### ### ### ###
    
    return Resources.special_condition(True, passed, mes)

### Execute test cases
try:
    passed1 = passed2 = passed3 = passed4 = passed5 = passed6 = passed7 = True
    passed1 = Resources.result(changing_units())
    passed2 = Resources.result(changing_name())
    passed3 = Resources.result(changing_color())
    passed4 = Resources.result(changing_opacity())
    passed5 = Resources.result(display_bounding_box())
    passed6 = Resources.result(visualization_and_validation())
    passed7 = Resources.result(custom_properties())
    ##### TEST CASES END #####

    ##### END #####
    Resources.close_cadex_app()
    Resources.finish_set(passed1, passed2, passed3, passed4, passed5, passed6, passed7)

except:
    tb = traceback.format_exc()
    Resources.except_set(tb)
