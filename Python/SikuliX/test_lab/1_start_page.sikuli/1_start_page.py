import sys
sys.path.append('../Resources')
import traceback
import Resources
import common
import images

##### START #####
TOTAL = 9
Resources.start_set('Start Page')

##### TEST CASES #####
### Open new file in recent files ###
def open_new_file_in_recent_files():
    test_name = 'OPEN NEW FILE IN RECENT FILES'
    Resources.announce_text(test_name+Resources.test_num(1, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.start_cadex_app()

    ### ### ### ### ### ###
    wait(images.new_file, 30)
    click(images.new_file)
    paste(common.CADEX_DIR+'models\\FZK Haus.ifc')
    wait(1)
    type(Key.ENTER)
    Resources.display_complete()
    
    result = Resources.validate_image('FZK_Haus', '_BRep', 'ifc', test_name+' BREP', '')
    ### ### ### ### ### ###
    
    Resources.close_cadex_app()
    return result

### Check previews models in recent files ###
def check_previews_models_in_recent_files():
    test_name = 'CHECK PREVIEWS MODELS IN RECENT FILES'
    Resources.announce_text(test_name+Resources.test_num(2, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.start_cadex_app()

    ### ### ### ### ### ###
    FZK_Haus_ifc = False
    if exists(images.check_previews_fzk_haus, 10):
        FZK_Haus_ifc = True
        Resources.html_report_add_comment('ok', '"FZK Haus.ifc" is found.')
    else:
        FZK_Haus_ifc = False
        Resources.html_report_add_comment('fail', '"FZK Haus.ifc" is not found.')

    CADEX_models = False
    if exists(images.check_previews_cadex_models, 10):
        CADEX_models = True
        Resources.html_report_add_comment('ok', '"C:/Program Files/CAD Exchanger/models/" is found.')
    else:
        CADEX_models = False
        Resources.html_report_add_comment('fail', '"C:/Program Files/CAD Exchanger/models/" is not found.')

    passed = False
    mes = ''
    if FZK_Haus_ifc == True and CADEX_models == True:
        passed = True
    else:
        passed = False
        mes = 'Not all elements are found!'
    ### ### ### ### ### ###
    
    Resources.close_cadex_app()
    return Resources.special_condition(True, passed, mes)

### Check open some videos in tutorials ###
def check_open_some_videos_in_tutorials():
    test_name = 'CHECK OPEN SOME VIDEOS IN TUTORIALS'
    Resources.announce_text(test_name+Resources.test_num(3, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.start_cadex_app()

    ### ### ### ### ### ###
    passed = False
    mes = ''
    if exists(images.tutorials, 10):
        passed = True
        print('[log] Tutorials section is found.')
        click(images.tutorials)
    else:
        passed = False
        mes = 'Something went wrong!'
        print('[log] Tutorials section is not found!')
    ### ### ### ### ### ###
    
    Resources.close_cadex_app()
    return Resources.special_condition(True, passed, mes)

### Open sample ASM Torsen Differential.stp ###
def open_sample_asm_torsen_differential():
    test_name = 'OPEN SAMPLE ASM TORSEN DIFFERENTIAL'
    Resources.announce_text(test_name+Resources.test_num(4, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.start_cadex_app()

    ### ### ### ### ### ###
    Resources.import_model('samples', images.model_samples_asm_torsen_differential, 'ASM Torsen Differential', 'stp')
    
    #click(images.repres_icon)
    #click(images.repres_brep_blue)
    #Resources.display_complete()
    #click(images.close_pinned_panel)
    hSC, passedATDBRep, m = Resources.validate_image('ASM_Torsen_Differential', '_BRep', 'stp', test_name+' BREP', '')

    click(images.repres_icon)
    click(images.repres_poly1)
    Resources.display_complete()
    click(images.close_pinned_panel)
    hSC, passedATDPoly1, m = Resources.validate_image('ASM_Torsen_Differential', '_Poly1', 'stp', test_name+' POLY1', '')
    
    passed, mes = Resources.special_condition_validate('image', passedATDBRep, passedATDPoly1)
    ### ### ### ### ### ###
    
    Resources.close_cadex_app()
    return Resources.special_condition(True, passed, mes)

### Open sample FZK Haus.ifc ###
def open_sample_fzk_haus():
    test_name = 'OPEN SAMPLE FZK HAUS'
    Resources.announce_text(test_name+Resources.test_num(5, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.start_cadex_app()

    ### ### ### ### ### ###
    Resources.import_model('samples', images.model_samples_fzk_haus, 'FZK Haus', 'ifc')

    #click(images.repres_icon)
    #click(images.repres_brep_blue)
    #Resources.display_complete()
    #mclick(images.close_pinned_panel)
    
    result = Resources.validate_image('FZK_Haus', '_BRep', 'ifc', test_name+' BREP', '')
    ### ### ### ### ### ###
    
    Resources.close_cadex_app()
    return result

### Open sample Heavy Lifting Tow Truck.CATPart ###
def open_sample_heavy_lifting_tow_truck():
    test_name = 'OPEN SAMPLE HEAVY LIFTING TOW TRUCK'
    Resources.announce_text(test_name+Resources.test_num(6, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.start_cadex_app()

    ### ### ### ### ### ###
    Resources.import_model('samples', images.model_samples_heavy_lifting_tow_truck, 'Heavy Lifting Tow Truck', 'CATPart')
    
    click(images.repres_icon)
    click(images.repres_brep)
    Resources.display_complete()
    click(images.close_pinned_panel)
    hSC, passedHLTTBRep, m = Resources.validate_image('Heavy_Lifting_Tow_Truck', '_BRep', 'CATPart', test_name+' BREP', '')

    click(images.repres_icon)
    click(images.repres_poly1)
    #Resources.display_complete()
    click(images.close_pinned_panel)
    hSC, passedHLTTPoly1, m = Resources.validate_image('Heavy_Lifting_Tow_Truck', '_Poly1', 'CATPart', test_name+' POLY1', '')
    
    passed, mes = Resources.special_condition_validate('image', passedHLTTBRep, passedHLTTPoly1)
    ### ### ### ### ### ###
    
    Resources.close_cadex_app()
    return Resources.special_condition(True, passed, mes)

### Open sample Machinist Jack.dwg ###
def open_sample_machinist_jack():
    test_name = 'OPEN SAMPLE MACHINIST JACK'
    Resources.announce_text(test_name+Resources.test_num(7, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.start_cadex_app()

    ### ### ### ### ### ###
    Resources.import_model('samples', images.model_samples_machinist_jack, 'Machinist Jack', 'dwg')
    Resources.display_complete()
    
    result = Resources.validate_image('Machinist_Jack', '', 'dwg', test_name, '')
    ### ### ### ### ### ###
    
    Resources.close_cadex_app()
    return result

### Open sample Mecanum Wheel Type-B.jt ###
def open_sample_mecanum_wheel_type_b():
    test_name = 'OPEN SAMPLE MECANUM WHEEL TYPE-B'
    Resources.announce_text(test_name+Resources.test_num(8, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.start_cadex_app()

    ### ### ### ### ### ###
    Resources.import_model('samples', images.model_samples_mecanum_wheel_type_b, 'Mecanum Wheel Type-B', 'jt')
    
    click(images.repres_icon)
    click(images.repres_brep_blue)
    Resources.display_complete()
    click(images.close_pinned_panel)
    hSC, passedMWTBBRep, m = Resources.validate_image('Mecanum_Wheel_Type-B', '_BRep', 'jt', test_name+' BREP', '')

    passedMWTBPoly = {
            '1': [False, images.repres_poly1],
            '2': [False, images.repres_poly2],
            '3': [False, images.repres_poly3]
    }

    for polyNum in passedMWTBPoly:
        click(images.repres_icon)
        click(passedMWTBPoly[polyNum][1])
        if polyNum != '1':
            Resources.display_complete()
        click(images.close_pinned_panel)
        hSC, passedMWTBPoly[polyNum][0], m = Resources.validate_image('Mecanum_Wheel_Type-B', '_Poly'+polyNum, 'jt', test_name+' POLY'+polyNum, '')

    passed, mes = Resources.special_condition_validate('image', passedMWTBBRep, passedMWTBPoly['1'][0], passedMWTBPoly['2'][0], passedMWTBPoly['3'][0])
    ### ### ### ### ### ###
    
    Resources.close_cadex_app()
    return Resources.special_condition(True, passed, mes)

### Open sample Rotary Tiller.sldprt ###
def open_sample_rotary_tiller():
    test_name = 'OPEN SAMPLE ROTARY TILLER'
    Resources.announce_text(test_name+Resources.test_num(9, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.start_cadex_app()

    ### ### ### ### ### ###
    Resources.import_model('samples', images.model_samples_rotary_tiller, 'Rotary Tiller', 'sldprt')

    click(images.repres_icon)
    click(images.repres_brep)
    Resources.display_complete()
    click(images.close_pinned_panel)
    hSC, passedRTBRep, m = Resources.validate_image('Rotary_Tiller', '_BRep', 'sldprt', test_name+' BREP', '')

    click(images.repres_icon)
    click(images.repres_poly1)
    #Resources.display_complete()
    click(images.close_pinned_panel)
    hSC, passedRTPoly1, m = Resources.validate_image('Rotary_Tiller', '_Poly1', 'sldprt', test_name+' POLY1', '')
    
    passed, mes = Resources.special_condition_validate('image', passedRTBRep, passedRTPoly1)
    ### ### ### ### ### ###
    
    Resources.close_cadex_app()
    return Resources.special_condition(True, passed, mes)

### Execute test cases
try:
    passed1 = passed2 = passed3 = passed4 = passed5 = passed6 = passed7 = passed8 = passed9 = True
    passed1 = Resources.result(open_new_file_in_recent_files())
    passed2 = Resources.result(check_previews_models_in_recent_files())
    passed3 = Resources.result(check_open_some_videos_in_tutorials())
    passed4 = Resources.result(open_sample_asm_torsen_differential())
    passed5 = Resources.result(open_sample_fzk_haus())
    passed6 = Resources.result(open_sample_heavy_lifting_tow_truck())
    passed7 = Resources.result(open_sample_machinist_jack())
    passed8 = Resources.result(open_sample_mecanum_wheel_type_b())
    passed9 = Resources.result(open_sample_rotary_tiller())
    ##### TEST CASES END #####
    
    ##### END #####
    Resources.finish_set(passed1, passed2, passed3, passed4, passed5, passed6, passed7, passed8, passed9)

except:
    tb = traceback.format_exc()
    Resources.except_set(tb)
# (FindFailed, NameError, TypeError)