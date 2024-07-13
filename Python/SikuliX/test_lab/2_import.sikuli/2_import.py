import sys
sys.path.append('../Resources')
import traceback
import Resources
import common
import images

##### START #####
TOTAL = 5
Resources.start_set('Import')
Resources.start_cadex_app()

##### TEST CASES #####
### Open model in the "Recent files" ###
def open_model_in_the_recent_files():
    test_name = 'OPEN MODEL IN THE RECENT FILES'
    Resources.announce_text(test_name+Resources.test_num(1, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)

    ### ### ### ### ### ###
    # Any recent model on Start page
    wait(images.check_previews_fzk_haus, 10)
    click(images.check_previews_fzk_haus)
    Resources.display_complete()
    hSC, passedRecent, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_FZK_HAUS), '_BRep', common.FORMAT_IFC, test_name+' RECENT', 'Recent model on Start page: ')
    Resources.back_to_start_page()

    # "+ New file" on Start page
    click(images.new_file)
    paste(common.MODELS_DIR+common.MODEL_ATD+'.'+common.FORMAT_STP)
    type(Key.ENTER)
    Resources.display_complete()
    hSC, passedNewFile, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep', common.FORMAT_STP, test_name+' NEW FILE', 'New file on Start page: ')

    # On Import panel
    Resources.to_import_panel()
    Resources.show_import_options('uncheck')
    Resources.add_to_current('uncheck')
    doubleClick(images.model_recent_asm_torsen_differential)
    click(images.close_pinned_panel)
    Resources.display_complete()
    hSC, passedImportPanel, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep', common.FORMAT_STP, test_name+' IMPORT PANEL', 'On Import panel: ')
    
    passed, mes = Resources.special_condition_validate('image', passedRecent, passedNewFile, passedImportPanel)
    ### ### ### ### ### ###
    
    return Resources.special_condition(True, passed, mes)

### Show import options button in the "Recent files" ###
def show_import_options_button_in_the_recent_files():
    test_name = 'SHOW IMPORT OPTIONS BUTTON IN THE RECENT FILES'
    Resources.announce_text(test_name+Resources.test_num(2, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)

    ### ### ### ### ### ###
    click(images.import_and_export_icon)
    click(images.import_and_export_panel_import)
    Resources.show_import_options('check')
    Resources.add_to_current('uncheck')
    click(images.open_new_file_blue)
    paste(common.MODELS_DIR+common.MODEL_ATD+'.'+common.FORMAT_STP)
    type(Key.ENTER)
    click(images.bodies_type_auto)
    click(images.bodies_type_auto_blue)
    click(images.switch_on)
    click(images.switch_off)
    click(images.import_button_blue)
    click(images.import_and_export_icon_blue)
    Resources.display_complete()

    result = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep', common.FORMAT_STP, test_name, '')
    Resources.back_to_start_page()
    ### ### ### ### ### ###
    
    return result

### Import a model through Drag and Drop ###
def import_a_model_through_drag_and_drop():
    test_name = 'IMPORT A MODEL THROUGH DRAG AND DROP'
    Resources.announce_text(test_name+Resources.test_num(3, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)

    ### ### ### ### ### ###
    # On Start page
    wait(images.new_file, 10)
    Resources.open_cadex_models_folder()
    dragDrop(images.explorer_model_asm_torsen_differential, images.samples)
    Resources.display_complete()
    hSC, passedOnStartPage, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep', common.FORMAT_STP, test_name+' START PAGE', 'On Start page: ')
    
    # On Viewer
    click(images.explorer_task_bar)
    dragDrop(images.explorer_model_fzk_haus, images.import_and_export_icon)
    Resources.close_cadex_models_folder()
    Resources.display_complete()
    hSC, passedOnViewer, m = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_FZK_HAUS), '_BRep', common.FORMAT_IFC, test_name+' VIEWER', 'On Viewer: ')
    
    passed, mes = Resources.special_condition_validate('image', passedOnStartPage, passedOnViewer)
    ### ### ### ### ### ###

    Resources.close_cadex_app()
    return Resources.special_condition(True, passed, mes)

### Import a model through system file manager ###
def import_a_model_through_system_file_manager():
    test_name = 'IMPORT A MODEL THROUGH SYSTEM FILE MANAGER'
    Resources.announce_text(test_name+Resources.test_num(4, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)

    ### ### ### ### ### ###
    Resources.open_cadex_models_folder()
    rightClick(images.explorer_model_asm_torsen_differential)
    click(images.context_menu_open_with)
    click(images.context_menu_open_with_more_apps)
    click(images.context_menu_open_with_always_use_checkbox_checked)
    mouseMove(images.context_menu_open_with_slider)
    wheel(images.context_menu_open_with_slider, WHEEL_DOWN, 8)
    click(images.context_menu_open_with_find_another_app)
    wait(1)
    paste(common.CADEX_EXE)
    type(Key.ENTER)
    Resources.display_complete()
        
    result = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD), '_BRep', common.FORMAT_STP, test_name, '')
    ### ### ### ### ### ###

    click(images.explorer_task_bar)
    Resources.close_cadex_models_folder()
    return result

### Adding a new model to current ###
def adding_a_new_model_to_current():
    test_name = 'ADDING A NEW MODEL TO CURRENT'
    Resources.announce_text(test_name+Resources.test_num(5, TOTAL), common.ANN_SWITCH, common.ANN_VOLUME)
    Resources.test_case_border('start', test_name)

    ### ### ### ### ### ###
    Resources.to_import_panel()
    Resources.show_import_options('uncheck')
    Resources.add_to_current('check')
    click(images.open_new_file_blue)
    paste(common.MODELS_DIR+common.MODEL_HLTT+'.'+common.FORMAT_CATPART)
    type(Key.ENTER)
    click(images.import_and_export_icon_blue)
    Resources.display_complete()
        
    result = Resources.validate_image(Resources.prepare_for_validate(common.MODEL_ATD+' And '+common.MODEL_HLTT+' BRep'), '', '', test_name, '')
    ### ### ### ### ### ###
    
    return result

### Execute test cases
try:
    passed1 = passed2 = passed3 = passed4 = passed5 = True
    passed1 = Resources.result(open_model_in_the_recent_files())
    passed2 = Resources.result(show_import_options_button_in_the_recent_files())
    passed3 = Resources.result(import_a_model_through_drag_and_drop())
    passed4 = Resources.result(import_a_model_through_system_file_manager())
    passed5 = Resources.result(adding_a_new_model_to_current())
    ##### TEST CASES END #####
    
    ##### END #####
    Resources.close_cadex_app()
    Resources.finish_set(passed1, passed2, passed3, passed4, passed5)

except:
    tb = traceback.format_exc()
    Resources.except_set(tb)
