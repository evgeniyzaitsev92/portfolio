import sys
sys.path.append('../Resources')
import traceback
import Resources
import common
import images

##### START #####
TOTAL = 5
Resources.start_set('Viewport')
Resources.start_cadex_app()

##### TEST CASES #####
def check_tooltip(display_mode, tooltip, mes):
        check = False
        mouseMove(display_mode)
        wait(1)
        if exists(Pattern(tooltip).similar(0.95)):
            check = True
            Resources.html_report_add_comment('ok', 'Display mode "%s" tooltip is found.'%mes)
        else:
            Resources.html_report_add_comment('fail', 'Display mode "%s" tooltip is not found!'%mes)
            
### Display mode ###
def display_mode():
    test_name = 'DISPLAY MODE'
    Resources.announce_text(test_name+Resources.test_num(1, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.import_model('samples', images.model_samples_asm_torsen_differential, common.MODEL_ATD, common.FORMAT_STP)
        
    ### ### ### ### ### ###
    ## BRep
    # Wireframe
    mouseMove(images.view_icon)
    passedBRepWireframeTT = check_tooltip(images.view_dm_wireframe, images.view_dm_wireframe_tt, 'Wireframe')
    click(images.view_dm_wireframe)
    hSC, passedBRepWireframe, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep_View_DM_Wireframe', common.FORMAT_STP, test_name+' BREP DM WIREFRAME', 'BRep display mode "Wireframe": ')

    # Shading
    mouseMove(images.view_icon)
    passedBRepShadingTT = check_tooltip(images.view_dm_shading, images.view_dm_shading_tt, 'Shading')
    click(images.view_dm_shading)
    hSC, passedBRepShading, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep_View_DM_Shading', common.FORMAT_STP, test_name+' BREP DM SHADING', 'BRep display mode "Shading": ')
    
    # Shading with boundaries
    mouseMove(images.view_icon)
    passedBRepShadingWithBoundariesTT = check_tooltip(images.view_dm_shading_with_boundaries, images.view_dm_shading_with_boundaries_tt, 'Shading with boundaries')
    click(images.view_dm_shading_with_boundaries)
    hSC, passedBRepShadingWithBoundaries, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep', common.FORMAT_STP, test_name+' BREP DM SHADING WITH BOUNDARIES', 'BRep display mode "Shading with boundaries": ')
    
    # Shading with surface boundaries
    mouseMove(images.view_icon)
    wait(1)
    passedBRepShadingWithSurfaceBoundaries = False
    if exists(Pattern(images.view_dm_shading_with_surface_boundaries_disabled).similar(0.95)):
        passedBRepShadingWithSurfaceBoundaries = True
        Resources.html_report_add_comment('ok', 'BRep display mode "Shading with boundaries" is disabled.')
    else:
        Resources.html_report_add_comment('fail', 'BRep display mode "Shading with boundaries" is not disabled!')

    ## Poly
    mouseMove(images.view_icon)
    click(images.view_dm_wireframe)
    click(images.repres_icon)
    click(images.repres_poly1)
    Resources.display_complete()
    click(images.close_pinned_panel)

    # Wireframe
    hSC, passedPoly1Wireframe, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_Poly1_View_DM_Wireframe', common.FORMAT_STP, test_name+' POLY1 DM WIREFRAME', 'Poly1 display mode "Wireframe": ')

    # Shading
    mouseMove(images.view_icon)
    click(images.view_dm_shading)
    hSC, passedPoly1Shading, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_Poly1_View_DM_Shading', common.FORMAT_STP, test_name+' POLY1 DM SHADING', 'Poly1 display mode "Shading": ')
    
    # Shading with boundaries
    mouseMove(images.view_icon)
    click(images.view_dm_shading_with_boundaries)
    hSC, passedPoly1ShadingWithBoundaries, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_Poly1', common.FORMAT_STP, test_name+' POLY1 DM SHADING WITH BOUNDARIES', 'Poly1 display mode "Shading with boundaries": ')
    
    # Shading with surface boundaries
    mouseMove(images.view_icon)
    passedPoly1ShadingWithSurfaceBoundariesTT = check_tooltip(images.view_dm_shading_with_surface_boundaries, images.view_dm_shading_with_surface_boundaries_tt, 'Shading with surface boundaries')
    click(images.view_dm_shading_with_surface_boundaries)
    hSC, passedPoly1ShadingWithSurfaceBoundaries, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_Poly1_View_DM_Shading_With_Surface_Boundaries', common.FORMAT_STP, test_name+' POLY1 DM SHADING WITH SURFACE BOUNDARIES', 'Poly1 display mode "Shading with surface boundaries": ')

    ## Result
    mouseMove(images.view_icon)
    click(images.view_dm_shading_with_boundaries)
    Resources.back_to_start_page()
    passed, mes = Resources.special_condition_validate('other', passedBRepWireframeTT, passedBRepWireframe, 
            passedBRepShadingTT, passedBRepShading, passedBRepShadingWithBoundariesTT, passedBRepShadingWithBoundaries,
            passedBRepShadingWithSurfaceBoundaries, passedPoly1Wireframe, passedPoly1Shading,
            passedPoly1ShadingWithBoundaries, passedPoly1ShadingWithSurfaceBoundariesTT, passedPoly1ShadingWithSurfaceBoundaries)
    ### ### ### ### ### ###

    return Resources.special_condition(True, passed, mes)

### Projection mode ###
def projection_mode():
    test_name = 'PROJECTION MODE'
    Resources.announce_text(test_name+Resources.test_num(2, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.import_model('samples', images.model_samples_asm_torsen_differential, common.MODEL_ATD, common.FORMAT_STP)
        
    ### ### ### ### ### ###
    ## Perspective
    mouseMove(images.view_icon)
    passedBRepPerspectiveTT = check_tooltip(images.view_pm_perspective, images.view_pm_perspective_tt, 'Perspective')
    click(images.view_pm_perspective)
    hSC, passedBRepPerspective, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep_View_PM_Perspective', common.FORMAT_STP, test_name+' BREP PM PERSPECTIVE', 'BRep projection mode "Perspective": ')

    ## Isometric
    mouseMove(images.view_icon)
    passedBRepIsometricTT = check_tooltip(images.view_pm_isometric, images.view_pm_isometric_tt, 'Isometric')
    click(images.view_pm_isometric)
    hSC, passedBRepIsometric, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep', common.FORMAT_STP, test_name+' BREP PM ISOMETRIC', 'BRep projection mode "Isometric": ')
    
    ## Result
    Resources.back_to_start_page()
    passed, mes = Resources.special_condition_validate('image', passedBRepPerspectiveTT, passedBRepPerspective, 
            passedBRepIsometricTT, passedBRepIsometric)
    ### ### ### ### ### ###

    return Resources.special_condition(True, passed, mes)

### Selection mode ###
def selection_mode():
    test_name = 'SELECTION MODE'
    Resources.announce_text(test_name+Resources.test_num(3, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.import_model('samples', images.model_samples_asm_torsen_differential, common.MODEL_ATD, common.FORMAT_STP)
        
    ### ### ### ### ### ###
    ## Poly
    click(images.repres_icon)
    click(images.repres_poly1)
    Resources.display_complete()
    mouseMove(images.view_icon)
    wait(1)
    passedPolySelectionModeDisabled = False
    if exists(Pattern(images.view_sm_disabled).similar(0.95)):
        passedPolySelectionModeDisabled = True
        Resources.html_report_add_comment('ok', 'Selection mode is disabled in Poly.')
    else:
        Resources.html_report_add_comment('fail', 'Selection mode is not disabled in Poly!')

    ## BRep
    click(images.repres_brep)
    click(images.close_pinned_panel)
    click(images.structure_icon)

    # Select by Body
    click(images.view_sm_part)
    wait(1)
    click(images.view_sm_part)
    wait(1)
    passedBRepBodyStructure = False
    if exists(Pattern(images.view_sm_body_structure).similar(0.95)):
        passedBRepBodyStructure = True
        Resources.html_report_add_comment('ok', 'Selection in view is synchronized with Structure panel.')
    else:
        Resources.html_report_add_comment('fail', 'Selection in view is not synchronized with Structure panel!')
    click(images.close_pinned_panel)
    hSC, passedBRepBody, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep_View_SM_Body', common.FORMAT_STP, test_name+' BREP SM BODY', 'Selection mode "Select by Body": ')
    click(images.viewport_bcg)

    # Select by Face
    mouseMove(images.view_icon)
    click(images.view_sm_select_by_face)
    click(images.view_sm_part)
    wait(1)
    click(images.view_sm_part)
    hSC, passedBRepFace, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep_View_SM_Face', common.FORMAT_STP, test_name+' BREP SM FACE', 'Selection mode "Select by Face": ')
    
    rightClick(images.view_sm_part_selected)
    passedBRepFaceHideDisabled = False
    if exists(Pattern(images.element_hide_disabled).similar(0.95)):
        passedBRepFaceHideDisabled = True
        Resources.html_report_add_comment('ok', '"Hide" option is disabled for faces.')
    else:
        Resources.html_report_add_comment('fail', '"Hide" option is not disabled for faces!')
    click(images.viewport_bcg)

    ## Result
    mouseMove(images.view_icon)
    click(images.view_sm_select_by_body)
    Resources.back_to_start_page()
    passed, mes = Resources.special_condition_validate('other', passedPolySelectionModeDisabled, passedBRepBodyStructure, 
            passedBRepBody, passedBRepFace, passedBRepFaceHideDisabled)
    ### ### ### ### ### ###

    return Resources.special_condition(True, passed, mes)

### Batch selection ###
def batch_selection():
    test_name = 'BATCH SELECTION'
    Resources.announce_text(test_name+Resources.test_num(4, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.import_model('samples', images.model_samples_asm_torsen_differential, common.MODEL_ATD, common.FORMAT_STP)
        
    ### ### ### ### ### ###
    keyDown(Key.CTRL)
    click(images.view_sm_part)
    click(images.view_sm_part2)
    keyUp(Key.CTRL)
    hSC, passedBRepBodyBatch, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep_View_SM_Body_Batch', common.FORMAT_STP, test_name+' BREP SM BODY BATCH', 'Batch selection "Select by Body": ')

    keyDown(Key.CTRL)
    click(images.view_sm_part2_selected)
    keyUp(Key.CTRL)
    hSC, passedBRepBodyBatch2, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep_View_SM_Body', common.FORMAT_STP, test_name+' BREP SM BODY BATCH2', 'Batch selection "Select by Body" deselect: ')
    click(images.viewport_bcg)

    ## Result
    Resources.back_to_start_page()
    passed, mes = Resources.special_condition_validate('image', passedBRepBodyBatch, passedBRepBodyBatch2)
    ### ### ### ### ### ###

    return Resources.special_condition(True, passed, mes)
    
### Execute test cases
try:
    passed1 = passed2 = passed3 = passed4 = True
    passed1 = Resources.result(display_mode())
    passed2 = Resources.result(projection_mode())
    passed3 = Resources.result(selection_mode())
    passed4 = Resources.result(batch_selection())
    ##### TEST CASES END #####
    
    ##### END #####
    Resources.close_cadex_app()
    Resources.finish_set(passed1, passed2, passed3, passed4)

except:
    tb = traceback.format_exc()
    Resources.except_set(tb)
