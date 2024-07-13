import sys
sys.path.append('../Resources')
import traceback
import Resources
import common
import images

##### START #####
TOTAL = 2
Resources.start_set('PMI')
Resources.start_cadex_app()

##### TEST CASES #####
### PMI tree view ###
def pmi_tree_view():
    test_name = 'PMI TREE VIEW'
    Resources.announce_text(test_name+Resources.test_num(1, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)
    Resources.import_model('new file', common.CADEX_SERVER+'TestModels\\JT\\NIST\\', 'nist_ctc_01_asme1_ct5210_rd', 'jt')
    
    ### ### ### ### ### ###
    click(images.structure_icon)
    doubleClick(images.structure_nist_ctc_01_asme1_ct5210_rd_jt)
    doubleClick(Pattern(images.structure_nist_ctc_01_asme1_ct5210_rd).similar(0.95))
    click(images.structure_pmi)
    wait(1)
    click(Pattern(images.checkbox_off).similar(0.95))

    # Check checkboxes - hide some parts and parents
    click(Pattern(images.pmi_tree_arrow).similar(0.95))
    click(images.pmi_tree_dim)
    wait(1)
    click(images.checkbox_off)
    wait(1)
    click(images.pmi_tree_feature)
    hSC, passedUncheckedFeature, m = Resources.validate_image('nist_ctc_01_asme1_ct5210_rd', '_PMI_Unchecked_Feature', 'jt', test_name+' UNCHECKED FEATURE', 'PMI Unchecked Feature: ')

    wait(1)
    click(images.checkbox_off)
    wait(1)
    click(images.pmi_tree_datum)
    wait(1)
    click(images.checkbox_off)
    wait(1)
    click(images.pmi_tree_note)
    wait(1)
    click(images.checkbox_off)

    # Hide some parts and parents when an element is selected
    rightClick(images.structure_nist_ctc_01_asme1_ct5210_rd_jt)
    wait(1)
    click(images.element_hide)
    hSC, passedHidden, m = Resources.validate_image('nist_ctc_01_asme1_ct5210_rd', '_PMI_Hidden', 'jt', test_name+' HIDDEN', 'PMI Hidden: ')
    rightClick(images.structure_nist_ctc_01_asme1_ct5210_rd_jt)
    click(images.element_show_all)

    # PMI when unselected
    hSC, passedUnselected, m = Resources.validate_image('nist_ctc_01_asme1_ct5210_rd', '_PMI_Unselected', 'jt', test_name+' UNSELECTED', 'PMI Unselected: ')

    # "Select an element..." warning
    wait(1)
    passedSelectAnElemWarning = False
    if exists(Pattern(images.pmi_select_an_elem_warn).similar(0.95)):
        passedSelectAnElemWarning = True
        Resources.html_report_add_comment('ok', '"Select an element..." warning exists.')
    else:
        Resources.html_report_add_comment('fail', '"Select an element..." warning does not exists!')

    # Check checkboxes using ctrl and shift
    click(Pattern(images.structure_nist_ctc_01_asme1_ct5210_rd).similar(0.95))
    click(Pattern(images.pmi_tree_arrow).similar(0.95))
    keyDown(Key.CTRL)
    click(images.pmi_tree_name_dim)
    click(images.pmi_tree_name_datum)
    keyUp(Key.CTRL)
    hSC, passedSelectedCtrl, m = Resources.validate_image('nist_ctc_01_asme1_ct5210_rd', '_PMI_Selected_Ctrl', 'jt', test_name+' SELECTED CTRL', 'PMI Selected Ctrl: ')

    click(images.pmi_tree_name_dim)
    keyDown(Key.SHIFT)
    click(images.pmi_tree_name_datum)
    keyUp(Key.SHIFT)
    hSC, passedSelectedShift, m = Resources.validate_image('nist_ctc_01_asme1_ct5210_rd', '_PMI_Selected_Shift', 'jt', test_name+' SELECTED SHIFT', 'PMI Selected Shift: ')

    type(Key.ESC)
    click(images.close_pinned_panel)
    Resources.back_to_start_page()

    passed, mes = Resources.special_condition_validate('other', passedUncheckedFeature, passedHidden, passedUnselected, passedSelectAnElemWarning, passedSelectedCtrl, passedSelectedShift)
    ### ### ### ### ### ###

    return Resources.special_condition(True, passed, mes)
    
### Execute test cases
try:
    passed1 = True
    passed1 = Resources.result(pmi_tree_view())
    #passed2 = Resources.result(replace_original())
    ##### TEST CASES END #####
    
    ##### END #####
    Resources.close_cadex_app()
    Resources.finish_set(passed1)

except:
    tb = traceback.format_exc()
    Resources.except_set(tb)
