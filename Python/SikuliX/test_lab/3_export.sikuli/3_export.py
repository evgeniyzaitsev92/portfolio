import sys
sys.path.append('../Resources')
import traceback
import Resources
import common
import images

##### START #####
TOTAL = 9
Resources.start_set('Export')
Resources.start_cadex_app()

Resources.import_model('samples', images.model_samples_asm_torsen_differential, common.MODEL_ATD, common.FORMAT_STP)

##### TEST CASES #####
### Export to available formats ###
def export_to_available_formats():
    test_name = 'EXPORT TO AVAILABLE FORMATS'
    Resources.announce_text(test_name+Resources.test_num(1, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)

    ### ### ### ### ### ###
    # Changing the color and transparency
    click(images.structure_icon)
    doubleClick(images.structure_asm_torsen_differential_stp)
    click(images.structure_asm_torsen_differential)
    click(images.properties_icon)
    click(images.properties_color)
    click(images.properties_color_html)
    for _ in range(8): type(Key.BACKSPACE)
    paste('#111111')
    type(Key.ENTER)
    click(images.properties_slider)
    for _ in range(5): type(Key.LEFT)
    type(Key.ESC)
    click(images.close_pinned_panel)

    # Export to formats
    Resources.export_model(images.formats_pdf, common.MODEL_ATD, 'pdf')
    Resources.export_model(images.formats_sat, common.MODEL_ATD, 'sat')
    Resources.export_model(images.formats_brep, common.MODEL_ATD, 'brep')
    Resources.export_model(images.formats_cdx, common.MODEL_ATD, 'cdx')
    Resources.export_model(images.formats_dae, common.MODEL_ATD, 'dae')
    Resources.export_model(images.formats_dxf, common.MODEL_ATD, 'dxf')
    Resources.export_model(images.formats_fbx, common.MODEL_ATD, 'fbx')
    Resources.export_model(images.formats_glb, common.MODEL_ATD, 'glb')
    Resources.export_model(images.formats_ifc, common.MODEL_ATD, 'ifc')
    Resources.export_model(images.formats_igs, common.MODEL_ATD, 'igs')
    Resources.export_model(images.formats_jt, common.MODEL_ATD, 'jt')
    Resources.export_model(images.formats_obj, common.MODEL_ATD, 'obj')
    Resources.export_model(images.formats_x_t, common.MODEL_ATD, 'x_t')
    Resources.export_model(images.formats_3dm, common.MODEL_ATD, '3dm')
    Resources.export_model(images.formats_stp, common.MODEL_ATD, 'stp')
    Resources.export_model(images.formats_stl, common.MODEL_ATD, 'stl')
    Resources.export_model(images.formats_u3d, common.MODEL_ATD, 'u3d')
    #Resources.export_model(images.formats_usd, common.MODEL_ATD, 'usd')
    Resources.export_model(images.formats_wrl, common.MODEL_ATD, 'wrl')
    Resources.export_model(images.formats_x3d, common.MODEL_ATD, 'x3d')

    # Check Export to BREP format
    Resources.import_model('menu', common.TEST_DIR+'output\\', 'ASM Torsen Differential', 'brep')
    hSC, passedBRep, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep_From_Brep', common.FORMAT_STP, test_name+' BREP FROM BREP', '')

    # Check Export to JT format
    Resources.import_model('menu', common.TEST_DIR+'output\\', 'ASM Torsen Differential', 'jt')
    hSC, passedJT, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_JT', common.FORMAT_STP, test_name+' JT', '')
    
    passed, mes = Resources.special_condition_validate('image', passedBRep, passedJT)
    Resources.back_to_start_page()
    ### ### ### ### ### ###
    
    return Resources.special_condition(True, passed, mes)

### Export to a path longer than 260 characters ###
def export_to_a_path_longer_than_260_characters():
    test_name = 'EXPORT TO A PATH LONGER THAN 260 CHARACTERS'
    Resources.announce_text(test_name+Resources.test_num(2, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.import_model('samples', images.model_samples_asm_torsen_differential, common.MODEL_ATD, common.FORMAT_STP)

    ### ### ### ### ### ###
    name = 'Теперь мой черед спросить вас «отчего», мой милый, – сказал Болконский. – Я вам признаюсь, что не понимаю, может быть, тут есть дипломатические тонкости выше моего слабого ума, но я не понимаю: Мак теряет целую армию, эрцгерцог Фердинанд и эрцгерцог Карл не дают никаких признаков жизни и делают ошибки за ошибками, наконец, один Кутузов одерживает действительную победу, уничтожает charme [ очарование ] французов, и военный министр не интересуется даже знать подробности'
    click(images.import_and_export_icon)
    Resources.wheel_mouse(images.formats_pdf)
    click(images.formats_pdf)
    Resources.show_export_options('uncheck')
    click(images.export)
    paste(common.TEST_DIR+'output\\'+name+'.pdf')
    type(Key.ENTER)
    
    passed = False
    mes = ''
    if exists(images.close_button, 10):
        passed = True
        Resources.html_report_add_comment('ok', 'Export is not performed due to a long name.')
        click(images.close_button)
    else:
        passed = False
        mes = 'Something went wrong!'
    ### ### ### ### ### ###
    
    return Resources.special_condition(True, passed, mes)

### Applying export formats filters  ###
def applying_export_formats_filters():
    test_name = 'APPLYING EXPORT FORMAT FILTERS'
    Resources.announce_text(test_name+Resources.test_num(3, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)

    ### ### ### ### ### ###
    click(images.filters_off)
    passed = False
    passedListOff = False
    passedListOn = False
    mes = ''

    if exists(Pattern(images.filters_brep_on).similar(0.95), 2):
        click(Pattern(images.filters_brep_on).similar(0.95))
        
    if exists(Pattern(images.filters_mesh_on).similar(0.95)):
        click(Pattern(images.filters_mesh_on).similar(0.95))
        
    if exists(Pattern(images.filters_drawings_on).similar(0.95)):
        click(Pattern(images.filters_drawings_on).similar(0.95))
        
    if exists(Pattern(images.filters_list_off).similar(0.95), 2):
        click(Pattern(images.filters_brep_off).similar(0.95))
        click(Pattern(images.filters_mesh_off).similar(0.95))
        click(Pattern(images.filters_drawings_off).similar(0.95))
        passedListOff = True
        Resources.html_report_add_comment('ok', 'Filters list unchecked is full.')
    else:
        passedListOff = False
        Resources.html_report_add_comment('fail', 'Filters list unchecked is NOT full!')

    if exists(Pattern(images.filters_list_on).similar(0.95), 2):
        click(Pattern(images.filters_brep_on).similar(0.95))
        click(Pattern(images.filters_mesh_on).similar(0.95))
        click(Pattern(images.filters_drawings_on).similar(0.95))
        passedListOn = True
        Resources.html_report_add_comment('ok', 'Filters list checked is full.')
    else:
        passedListOn = False
        Resources.html_report_add_comment('fail', 'Filters list checked is NOT full!')

    if passedListOff == True and passedListOn == True:
        passed = True
    else:
        passed = False
        mes = 'Something is wrong with filter list!'

    click(images.import_and_export_icon_blue)
    ### ### ### ### ### ###
    
    return Resources.special_condition(True, passed, mes)

### Export model with hidden items ###
def export_model_with_hidden_items():
    test_name = 'EXPORT MODEL WITH HIDDEN ITEMS'
    Resources.announce_text(test_name+Resources.test_num(4, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)

    ### ### ### ### ### ###
    click(images.structure_icon)
    doubleClick(images.structure_asm_torsen_differential_stp)
    doubleClick(images.structure_asm_torsen_differential)
    click(Pattern(images.structure_asm_torsen_differential_torsen_differential_v3).similar(0.95))
    rightClick(images.structure_asm_torsen_differential_torsen_differential_v3)
    click(images.element_hide)
    click(images.close_pinned_panel)
        
    result = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep_Hidden', common.FORMAT_STP, test_name, 'Hidden: ')
    Resources.back_to_start_page()
    ### ### ### ### ### ###
    
    return result

### Export model with changed names ###
def export_model_with_changed_names():
    test_name = 'EXPORT MODEL WITH CHANGED NAMES'
    Resources.announce_text(test_name+Resources.test_num(5, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.import_model('samples', images.model_samples_asm_torsen_differential, common.MODEL_ATD, common.FORMAT_STP)

    ### ### ### ### ### ###
    wait(images.structure_icon, FOREVER)
    click(images.structure_icon)
    doubleClick(images.structure_asm_torsen_differential_stp)
    doubleClick(Pattern(images.structure_asm_torsen_differential).similar(0.95))
    click(Pattern(images.structure_asm_torsen_differential_torsen_differential_v3).similar(0.95))
    click(images.properties_icon)
    click(Pattern(images.properties_edit_name).similar(0.95))
    for _ in range(50): type(Key.BACKSPACE)
    paste('new_name')
    type(Key.ENTER)
    type(Key.ESC)
    click(images.close_pinned_panel)
    Resources.export_model(images.formats_stp, common.MODEL_ATD+'2', common.FORMAT_STP)
    Resources.import_model('menu', common.TEST_DIR+'output\\', common.MODEL_ATD+'2', common.FORMAT_STP)
    wait(images.structure_icon, FOREVER)
    click(images.structure_icon)
    doubleClick(images.structure_asm_torsen_differential_stp)
    doubleClick(images.structure_asm_torsen_differential)
    
    passed = False
    mes = ''
    if exists(images.structure_check_new_name, 10):
        passed = True
        Resources.html_report_add_comment('ok', 'The name was successfully changed.')
    else:
        passed = False
        mes = 'Obviously the name was not successfully changed!'

    click(images.close_pinned_panel)
    Resources.back_to_start_page()
    ### ### ### ### ### ###
    
    return Resources.special_condition(True, passed, mes)

### Export model when explode is active ###
def export_model_when_explode_is_active():
    test_name = 'EXPORT MODEL WHEN EXPLODE IS ACTIVE'
    Resources.announce_text(test_name+Resources.test_num(6, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.import_model('samples', images.model_samples_asm_torsen_differential, common.MODEL_ATD, common.FORMAT_STP)

    ### ### ### ### ### ###
    mouseMove(Pattern(images.explode).similar(0.95))
    click(images.explode_edit)
    type('6')
    type(Key.ENTER)
    hSC, passedExplode, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep_Explode', common.FORMAT_STP, test_name+' BREP EXPLODE', 'Explode: ')
    Resources.export_model(images.formats_ifc, common.MODEL_ATD+'2', common.FORMAT_IFC)
    Resources.import_model('menu', common.TEST_DIR+'output\\', common.MODEL_ATD+'2', common.FORMAT_IFC)
    hSC, passedNoExplode, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_IFC', common.FORMAT_IFC, test_name+' IFC', 'No Explode: ')
    
    passed, mes = Resources.special_condition_validate('image', passedExplode, passedNoExplode)
    Resources.back_to_start_page()
    ### ### ### ### ### ###
    
    return Resources.special_condition(True, passed, mes)

### Export model with changed transformations ###
def export_model_when_changed_transformations():
    test_name = 'EXPORT MODEL WHEN CHANGED TRANSFORMATIONS'
    Resources.announce_text(test_name+Resources.test_num(7, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.import_model('samples', images.model_samples_asm_torsen_differential, common.MODEL_ATD, common.FORMAT_STP)

    ### ### ### ### ### ###
    click(images.manipulator)
    click(Pattern(images.manipulator_element_for_move).similar(0.95))
    click(Pattern(images.manipulator_arrow).similar(0.95))
    dragDrop(Pattern(images.manipulator_arrow_hover).similar(0.95), Pattern(images.properties_icon).similar(0.95))
    click(images.apply_button)
    Resources.export_model(images.formats_ifc, common.MODEL_ATD+'3', common.FORMAT_IFC)
    Resources.import_model('menu', common.TEST_DIR+'output\\', common.MODEL_ATD+'3', common.FORMAT_IFC)

    result = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_IFC_Manipulator', common.FORMAT_IFC, test_name, 'Manipulator transformation: ')
    Resources.back_to_start_page()
    ### ### ### ### ### ###
    
    return result

### Export model with PMI ###
def export_model_with_pmi():
    test_name = 'EXPORT MODEL WITH PMI'
    Resources.announce_text(test_name+Resources.test_num(8, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.import_model('new file', common.CADEX_SERVER+'TestModels\\JT\\NIST\\', 'nist_ctc_01_asme1_ct5210_rd', 'jt')

    ### ### ### ### ### ###
    click(images.structure_icon)
    doubleClick(images.structure_nist_ctc_01_asme1_ct5210_rd_jt)
    click(Pattern(images.structure_nist_ctc_01_asme1_ct5210_rd).similar(0.95))
    click(images.structure_pmi)
    click(Pattern(images.checkbox_off).similar(0.95))
    type(Key.ESC)
    click(images.close_pinned_panel)
    hSC, passedJT, m = Resources.validate_image('nist_ctc_01_asme1_ct5210_rd', '_PMI', 'jt', test_name+' JT', 'PMI in JT: ')

    ## Export to STEP
    wait(images.import_and_export_icon, FOREVER)
    click(images.import_and_export_icon)

    Resources.wheel_mouse(images.formats_stp)
    click(images.formats_stp)
    Resources.show_export_options('check')
    click(images.export)
    paste(common.OUTPUT_DIR+'nist_ctc_01_asme1_ct5210_rd.stp')
    type(Key.ENTER)

    if not exists(Pattern(images.pmi_write_switch_on).similar(0.95), 10):
        click(images.pmi_write_switch)
        click(Location(Env.getMouseLocation().getX(), Env.getMouseLocation().getY()).offset(500, 0))

    click(Pattern(images.export_in_options).similar(0.95))
    click(images.import_and_export_icon_blue)
    wait(images.export_completed, FOREVER)
    click(images.export_completed)
   
    ## Export to ACIS
    wait(images.import_and_export_icon, FOREVER)
    click(images.import_and_export_icon)

    Resources.wheel_mouse(images.formats_sat)
    click(images.formats_sat)
    Resources.show_export_options('check')
    click(images.export)
    paste(common.OUTPUT_DIR+'nist_ctc_01_asme1_ct5210_rd.sat')
    type(Key.ENTER)

    if not exists(Pattern(images.pmi_write_switch).similar(0.95), 10):
        click(Pattern(images.export_in_options).similar(0.95))
        
    click(images.import_and_export_icon_blue)
    wait(images.export_completed, FOREVER)
    click(images.export_completed)

    ## Check PMI in STEP
    Resources.import_model('menu', common.OUTPUT_DIR, 'nist_ctc_01_asme1_ct5210_rd', 'stp')
    click(images.structure_icon)
    doubleClick(images.structure_nist_ctc_01_asme1_ct5210_rd_stp)
    click(Pattern(images.structure_nist_ctc_01_asme1_ct5210_rd).similar(0.95))
    click(images.structure_pmi)
    click(Pattern(images.checkbox_off).similar(0.95))
    type(Key.ESC)
    click(images.close_pinned_panel)
    hSC, passedSTP, m = Resources.validate_image('nist_ctc_01_asme1_ct5210_rd', '_STP_PMI', 'stp', test_name+' STP', 'PMI in STP: ')
    
    ## Check PMI in ACIS
    Resources.import_model('menu', common.OUTPUT_DIR, 'nist_ctc_01_asme1_ct5210_rd', 'sat')
    click(images.structure_icon)
    doubleClick(images.structure_nist_ctc_01_asme1_ct5210_rd_sat)
    click(Pattern(images.structure_nist_ctc_01_asme1_ct5210_rd).similar(0.95))
    click(images.properties_icon)
    passedSAT = False
    wait(1)
    if exists(Pattern(images.pmi_does_not_contain).similar(0.95)) or exists(Pattern(images.pmi_does_not_contain2).similar(0.95)):
        passedSAT = True
        Resources.html_report_add_comment('ok', 'ACIS does not contain PMI.')
    else:
        Resources.html_report_add_comment('fail', 'Obviously ACIS contains PMI!')

    ## Result
    passed = False
    mes = ''
    if passedJT == True and passedSTP == True and passedSAT == True:
        passed = True
    else:
        passed = False
        mes = 'Something went wrong!'

    type(Key.ESC)
    click(images.close_pinned_panel)
    Resources.back_to_start_page()
    ### ### ### ### ### ###
    
    return Resources.special_condition(True, passed, mes)

### Export Poly to BRep ###
def export_poly_to_brep():
    test_name = 'EXPORT POLY TO BREP'
    Resources.announce_text(test_name+Resources.test_num(9, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.import_model('new file', common.CADEX_SERVER+'TestModels\\OBJ\\grabcad.com\\', 'Yamaha_YZF_R1', 'obj')

    ### ### ### ### ### ###
    wait(images.import_and_export_icon, FOREVER)
    click(images.import_and_export_icon)
    Resources.wheel_mouse(images.formats_sat)
    click(images.formats_sat)
    Resources.show_export_options('uncheck')
    click(images.export)
    passedWarning = False
    if exists(images.poly_to_brep_more_info, FOREVER):
        passedWarning = True
        Resources.html_report_add_comment('ok', 'Poly to BRep warning exists.')
        click(images.poly_to_brep_more_info)
    else:
        Resources.html_report_add_comment('fail', 'Poly to BRep warning does not exist!')
    
    passedArticle = False
    if exists(images.poly_to_brep_article, FOREVER):
        passedArticle = True
        Resources.html_report_add_comment('ok', 'Poly to BRep article is opened.')
    else:
        Resources.html_report_add_comment('fail', 'Poly to BRep article is not opened.')
        
    Resources.APP.focus()
    click(images.proceed_button)
    paste(common.OUTPUT_DIR+'Yamaha_YZF_R1.sat')
    type(Key.ENTER)
    click(images.import_and_export_icon_blue)
    wait(images.export_completed, FOREVER)
    click(images.export_completed)

    ## Result
    passed = False
    mes = ''
    if passedWarning == True and passedArticle == True:
        passed = True
    else:
        passed = False
        mes = 'Something went wrong!'
    ### ### ### ### ### ###
    
    return Resources.special_condition(True, passed, mes)

### Execute test cases
try:
    passed1 = passed2 = passed3 = passed4 = passed5 = passed6 = passed7 = passed8 = passed9 = True
    passed1 = Resources.result(export_to_available_formats())
    passed2 = Resources.result(export_to_a_path_longer_than_260_characters())
    passed3 = Resources.result(applying_export_formats_filters())
    passed4 = Resources.result(export_model_with_hidden_items())
    passed5 = Resources.result(export_model_with_changed_names())
    passed6 = Resources.result(export_model_when_explode_is_active())
    passed7 = Resources.result(export_model_when_changed_transformations())
    passed8 = Resources.result(export_model_with_pmi())
    passed9 = Resources.result(export_poly_to_brep())
    ##### TEST CASES END #####
    
    ##### END #####
    Resources.close_cadex_app()
    Resources.finish_set(passed1, passed2, passed3, passed4, passed5, passed6, passed7, passed8, passed9)

except:
    tb = traceback.format_exc()
    Resources.except_set(tb)
