import sys
sys.path.append('../Resources')
import traceback
import Resources
import common
import images

##### START #####
TOTAL = 2
Resources.start_set('Mesh decimation')
Resources.start_cadex_app()

##### TEST CASES #####
### Simplify ###
def simplify():
    test_name = 'SIMPLIFY'
    Resources.announce_text(test_name+Resources.test_num(1, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.import_model('samples', images.model_samples_asm_torsen_differential, common.MODEL_ATD, common.FORMAT_STP)
    
    ### ### ### ### ### ###
    click(images.repres_icon)

    # BRep
    wait(Pattern(images.repres_options_blue).similar(0.95), 10)
    click(Pattern(images.repres_options_blue).similar(0.95))
    passedNoSimplifyForBRep = False
    if not exists(images.repres_options_blue_simplify, 2):
        passedNoSimplifyForBRep = True
        Resources.html_report_add_comment('ok', '"Simplify" option does not exist for BRep.')
    else:
        passedNoSimplifyForBRep = False
        Resources.html_report_add_comment('fail', '"Simplify" option obviously exists for BRep!')

    # Poly 1  - Dec 1
    click(images.repres_poly1)
    Resources.display_complete()
    click(Pattern(images.repres_options_blue).similar(0.95))
    click(images.repres_options_blue_simplify)
    wait(images.repres_mesh_add, 10)
    if not exists(images.repres_simplify_dod_low, 2):
        click(images.repres_simplify_dod_arrow)
        click(images.repres_simplify_dod_low_blue)
    click(images.repres_mesh_add)
    Resources.display_complete()
    passedPoly1Dec1 = False
    if exists(images.repres_poly1_dec1, 2):
        passedPoly1Dec1 = True
        Resources.html_report_add_comment('ok', 'Polygonal 1 Decimated 1 is OK.')
    else:
        passedPoly1Dec1 = False
        Resources.html_report_add_comment('fail', 'Polygonal 1 Decimated 1 is not OK!')

    # Poly 1  - Dec 2
    click(images.repres_poly1)
    click(Pattern(images.repres_options_blue).similar(0.95))
    click(images.repres_options_blue_simplify)
    wait(images.repres_mesh_add, 10)
    click(images.repres_simplify_dod_arrow)
    click(images.repres_simplify_dod_medium)
    click(images.repres_mesh_add)
    Resources.display_complete()
    passedPoly1Dec2 = False
    if exists(images.repres_poly1_dec2, 2):
        passedPoly1Dec2 = True
        Resources.html_report_add_comment('ok', 'Polygonal 1 Decimated 2 is OK.')
    else:
        passedPoly1Dec2 = False
        Resources.html_report_add_comment('fail', 'Polygonal 1 Decimated 2 is not OK!')

    # Poly 1  - Dec 3
    click(images.repres_poly1)
    click(Pattern(images.repres_options_blue).similar(0.95))
    click(images.repres_options_blue_simplify)
    wait(images.repres_mesh_add, 10)
    click(images.repres_simplify_dod_arrow)
    click(images.repres_simplify_dod_high)
    click(images.repres_mesh_add)
    Resources.display_complete()
    passedPoly1Dec3 = False
    if exists(images.repres_poly1_dec3, 2):
        passedPoly1Dec3 = True
        Resources.html_report_add_comment('ok', 'Polygonal 1 Decimated 3 is OK.')
    else:
        passedPoly1Dec3 = False
        Resources.html_report_add_comment('fail', 'Polygonal 1 Decimated 3 is not OK!')

    # Poly 1  - Dec 4
    click(images.repres_poly1)
    click(Pattern(images.repres_options_blue).similar(0.95))
    click(images.repres_options_blue_simplify)
    wait(images.repres_mesh_add, 10)
    click(images.repres_simplify_dod_arrow)
    click(images.repres_simplify_dod_custom)
    click(images.repres_simplify_dod_custom_edit)
    for _ in range(7): type(Key.BACKSPACE)
    paste('2')
    type(Key.ENTER)
    click(images.repres_mesh_add)
    Resources.display_complete()
    passedPoly1Dec4 = False
    if exists(images.repres_poly1_dec4, 2):
        passedPoly1Dec4 = True
        Resources.html_report_add_comment('ok', 'Polygonal 1 Decimated 4 is OK.')
    else:
        passedPoly1Dec4 = False
        Resources.html_report_add_comment('fail', 'Polygonal 1 Decimated 4 is not OK!')

    passed = False
    mes = ''
    if passedNoSimplifyForBRep == True and passedPoly1Dec1 == True and passedPoly1Dec2 == True and passedPoly1Dec3 == True and passedPoly1Dec4 == True:
        passed = True
    else:
        passed = False
        mes = 'Something went wrong!'

    click(images.close_pinned_panel)
    Resources.back_to_start_page()
    ### ### ### ### ### ###

    return Resources.special_condition(True, passed, mes)

### Replace original ###
def replace_original():
    test_name = 'REPLACE ORIGINAL'
    Resources.announce_text(test_name+Resources.test_num(2, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.import_model('samples', images.model_samples_asm_torsen_differential, common.MODEL_ATD, common.FORMAT_STP)
    
    ### ### ### ### ### ###
    click(images.repres_icon)
    click(images.repres_poly1)
    Resources.display_complete()
    click(Pattern(images.repres_options_blue).similar(0.95))
    click(images.repres_options_blue_simplify)
    wait(images.repres_mesh_add, 10)
    if not exists(images.repres_simplify_dod_low, 2):
        click(images.repres_simplify_dod_arrow)
        click(images.repres_simplify_dod_low_blue)

    if not exists(Pattern(images.repres_replace_original_switch_on).similar(0.95), 10):
        click(images.repres_replace_original_switch)
        click(Location(Env.getMouseLocation().getX(), Env.getMouseLocation().getY()).offset(490, 0))
    
    click(images.repres_mesh_add)
    Resources.display_complete()
    passedPoly1Dec1 = False
    if exists(images.repres_poly1_dec1, 2):
        passedPoly1Dec1 = True
        Resources.html_report_add_comment('ok', 'Polygonal 1 Decimated 1 is OK.')
    else:
        passedPoly1Dec1 = False
        Resources.html_report_add_comment('fail', 'Polygonal 1 Decimated 1 is not OK!')

    passed = False
    mes = ''
    if passedPoly1Dec1 == True:
        passed = True
    else:
        passed = False
        mes = 'Something went wrong!'
    ### ### ### ### ### ###

    return Resources.special_condition(True, passed, mes)
    
### Execute test cases
try:
    passed1 = passed2 = True
    passed1 = Resources.result(simplify())
    passed2 = Resources.result(replace_original())
    ##### TEST CASES END #####
    
    ##### END #####
    Resources.close_cadex_app()
    Resources.finish_set(passed1, passed2)

except:
    tb = traceback.format_exc()
    Resources.except_set(tb)
