from sikuli import *
import os
import shutil
import subprocess
import traceback
import common
import images

Settings.MoveMouseDelay = 1
#os.system("start /wait cmd /c python d:\\CADEX\\Testing\\test_lab\\Resources.sikuli\\compare_images.py")
## VARIABLES
APP = App(common.CADEX_EXE)
EXPLORER = App('C:\\Windows\\explorer.exe')
WHEEL_COUNT = 0

## FUNCTIONS  
            
#subprocess.check_output('python '+common.CADEX_DIR+'Resources.sikuli\\compare_images.py '+test_name, shell=True)
#print(subprocess.check_output('python d:\\CADEX\\Testing\\test_lab\\Resources.sikuli\\compare_images.py FZK_Haus_BRep open_new_file_in_recent_files', shell=True))
    
# Start CAD Exchanger
def start_cadex_app():
    print('[cf] Start CAD Exchanger Lab')
    type('r', KeyModifier.WIN)
    paste('explorer '+common.CADEX_DIR+'bin\\Exchanger.exe')
    type(Key.ENTER)
    #APP.open()

# Close CAD Exchanger
def close_cadex_app():
    print('[cf] Close CAD Exchanger Lab')
    APP.focus()
    APP.close()
    if exists(images.exit_button):
        click(images.exit_button)

# Open CAD Exchanger models folder
def open_cadex_models_folder():
    print('[cf] Open CAD Exchanger models folder')
    type('r', KeyModifier.WIN)
    paste('explorer '+common.MODELS_DIR)
    type(Key.ENTER)

# Close CAD Exchanger models folder
def close_cadex_models_folder():
    print('[cf] Close CAD Exchanger models folder')
    click(images.close_explorer)
    #rightClick(Pattern(images.explorer_models).similar(0.8))
    #for _ in range(6): type(Key.DOWN)
    #type(Key.ENTER)

# Create report directory
def create_report_dir():
    if os.path.exists(common.REPORT_DIR):
        shutil.rmtree(common.REPORT_DIR)
    os.makedirs(common.REPORT_DIR)  

# Output directory
def output_dir(action):
    if action == 'create':
        if not os.path.exists(common.OUTPUT_DIR):
            print('[cf] Create output directory')
            os.makedirs(common.OUTPUT_DIR)
    elif action == 'delete':
        if os.path.exists(common.OUTPUT_DIR):
            print('[cf] Delete output directory')
            shutil.rmtree(common.OUTPUT_DIR)

def announce_text(text_to_announce, ann_switch, ann_volume):
    subprocess.check_output('python '+common.TEST_DIR+'Resources.sikuli\\announce_text.py '+text_to_announce.lower().replace(' ', '_')+' '+ann_switch+' '+str(ann_volume), shell=True)

def test_num(current, total):
    return ', %s of %s'%(str(current), str(total))
    
def resultAnswer(t, mes):
    if t == 'ok':
        print('[result] OK! Successfully passed!')
        return '<div class="result-answer"><h2 class="clr-green">OK!</h2><h2>Successfully passed!</h2></div>'
    elif t == 'fail':
        print('[result] FAIL! %s'%mes)
        return '<div class="result-answer"><h2 class="clr-red">FAIL!</h2><h2>%s</h2></div>'%mes
           
# Special condition
def special_condition(hasSpecialCondition, passed, mes):
    return hasSpecialCondition, passed, mes

# Special condition special_condition_validate_image
def special_condition_validate(t, *args):
    passed = True
    mes = ''
    for arg in args:
        if arg == False:
            passed = False
            if t == 'image':
                mes = 'Not all pairs of images are identical!'
            elif t == 'other':
                mes = 'Something went wrong!'
            break
        
    return passed, mes
    
# Validate model image (screenshot)
def validate_image(model_name, suffix, format_name, test_name, comment):
    test_name = test_name.lower().replace(' ', '_')
    print('[cf] Validate model image of "%s.%s"'%(model_name.replace('_', ' '), format_name))
    wait(images.import_and_export_icon, 10)
    click(images.import_and_export_icon)

    if exists(images.formats_image, 1):
        click(images.formats_image)
    else:
        if format_name != 'dwg':
            # Make slider appear
            mouseMove(Pattern(images.import_and_export_panel_vs_viewer).similar(0.95))
            mouseMove(Pattern(images.slider).similar(0.95))
            wheel(Pattern(images.slider).similar(0.95), WHEEL_DOWN, 8)
            click(images.formats_image)
            
    show_export_options('uncheck')
    click(images.export)
    paste(common.TEST_DIR+'output\\'+model_name.replace(' ', '_')+suffix+'.jpg')
    type(Key.ENTER)
    click(images.import_and_export_icon_blue)
    if exists(Pattern(images.export_completed).similar(0.95), 10):
        click(Pattern(images.export_completed).similar(0.95))
    similarity = subprocess.check_output('python '+common.TEST_DIR+'Resources.sikuli\\compare_images.py '+model_name.replace(' ', '_')+suffix+' '+test_name, shell=True)
    print('[log] Similarity: %s%%'%str(similarity)[:-2])
    common.HTML_REPORT += '<h2>** %s%s.%s **</h2>'%(comment, model_name.replace('_', ' '), format_name)
    common.HTML_REPORT += '<img src="%s.png" class="validate-image-img">'%(test_name)

    passed = False
    #mes = ''
    if float(similarity) >= 90.0:
        passed = True
        html_report_add_comment('ok', 'Images are fully or almost identical.')
    else:
        passed = False
        html_report_add_comment('fail', 'Output image differs from the original one much!')

    return special_condition(True, passed, '')

def result(func):
    #try:
    hasSpecialCondition, passed, mes = func
    if hasSpecialCondition == True:
        if passed == True:
            common.HTML_REPORT += resultAnswer('ok', '')
        else:
            common.HTML_REPORT += resultAnswer('fail', mes)
    else:
        passed = True
        common.HTML_REPORT += resultAnswer('ok', '')
           
    #except Exception as e:
    #    print('Except: '+repr(e))
    #    common.HTML_REPORT += resultAnswer('fail', repr(e))
    #    announce_text('fail, autotest is stopped', common.ANN_SWITCH, common.ANN_VOLUME)
        
    test_case_border('end', '')
    return passed

def html_report(action, main_name):
    if action == 'begin':
        common.HTML_REPORT += '''<html>
<head>
<link rel="stylesheet" href="../Resources.sikuli/styles.css">
<title>%s</title>
</head>
<body>
<center>
'''%(main_name)
        
    elif action == 'end':
        common.HTML_REPORT += '''</center>
</body>
</html>           
'''
        file_html = open(common.HTML_REPORT_FILE, 'w')
        file_html.write(common.HTML_REPORT)
        file_html.close()

def html_report_add_comment(t, comment):
    print('[log] %s'%comment)
    if t == 'ok':
        common.HTML_REPORT += '<div class="comment"><h2 class="clr-green comment--sign">+</h2><h3>%s</h3></div>'%comment
    elif t == 'fail':
        common.HTML_REPORT += '<div class="comment"><h2 class="clr-red comment--sign">-</h2><h3>%s</h3></div>'%comment
        
# Test case border
def test_case_border(action, test_name):
    if action == 'start':
        print('*** %s ***'%(test_name))
        common.HTML_REPORT += '<div class="test-case">' # test case div open
        common.HTML_REPORT += '<h2>*** %s ***</h2>'%(test_name)
    
    elif action == 'end':
        common.HTML_REPORT += '</div>'

def display_complete():
    wait(Pattern(images.display_complete).similar(0.95), FOREVER)
    click(Pattern(images.display_complete).similar(0.95))

def back_to_start_page():
    print('[cf] Back to Start page')
    click(images.menu_icon)
    wait(1)
    if exists(Pattern(images.back_to_start_page).similar(0.95), 1):
        click(Pattern(images.back_to_start_page).similar(0.95))
    elif exists(Pattern(images.back_to_start_page_blue).similar(0.95), 1):
        click(Pattern(images.back_to_start_page_blue).similar(0.95))
        
    if exists(images.recent_files, 2):
        click(images.recent_files)

def to_import_panel():
    print('[cf] To Import panel')
    click(images.menu_icon)
    click(images.menu_import)

def show_import_options(action):
    if action == 'check':
        if exists(Pattern(images.show_import_options_unchecked).similar(0.95)):
            click(Pattern(images.show_import_options_unchecked).similar(0.95))
    elif action == 'uncheck':
        if exists(Pattern(images.show_import_options_checked).similar(0.95)):
            click(Pattern(images.show_import_options_checked).similar(0.95))

def show_export_options(action):
    if action == 'check':
        if exists(Pattern(images.show_export_options_unchecked).similar(0.95)):
            click(Pattern(images.show_export_options_unchecked).similar(0.95))
    elif action == 'uncheck':
        if exists(Pattern(images.show_export_options_checked).similar(0.95)):
            click(Pattern(images.show_export_options_checked).similar(0.95))

def add_to_current(action):
    if action == 'check':
        if exists(Pattern(images.add_to_current_unchecked).similar(0.95)):
            click(Pattern(images.add_to_current_unchecked).similar(0.95))
    elif action == 'uncheck':
        if exists(Pattern(images.add_to_current_checked).similar(0.95)):
            click(Pattern(images.add_to_current_checked).similar(0.95))

def prepare_for_validate(model):
    print('[cf] Prepere for validate: %s'%model)
    return model.replace(' ', '_')

def wheel_mouse(image_path):
    # Make slider appear
    mouseMove(Pattern(images.import_and_export_panel_vs_viewer).similar(0.95))
    mouseMove(Pattern(images.slider).similar(0.95))
    
    WHEEL_COUNT = 0
    while 1:
        if not exists(image_path):
            if WHEEL_COUNT >= 0 and WHEEL_COUNT < 2:
                wheel(Pattern(images.slider).similar(0.95), WHEEL_DOWN, 3)
            elif WHEEL_COUNT >= 2 and WHEEL_COUNT < 4:
                wheel(Pattern(images.slider).similar(0.95), WHEEL_UP, 3)
            WHEEL_COUNT += 1
            if WHEEL_COUNT == 4:
                WHEEL_COUNT = 0
            wait(1)
        else:
            break

def export_model(image_path, model_name, format_name):
    print('[cf] Export to: %s.%s'%(model_name, format_name))
    wait(images.import_and_export_icon, 10)
    click(images.import_and_export_icon)

    if exists(image_path, 1):
        click(image_path)
    else:
        wheel_mouse(image_path)
        click(image_path)
        
    show_export_options('uncheck')
    click(images.export)
    paste(common.TEST_DIR+'output\\'+model_name+'.'+format_name)
    type(Key.ENTER)
    click(images.import_and_export_icon_blue)
    wait(Pattern(images.export_completed).similar(0.95), FOREVER)
    click(Pattern(images.export_completed).similar(0.95))

def import_model(how, path, model_name, format_name):
    print('[cf] Import of model: %s.%s'%(model_name, format_name))
    if how == 'samples':
        wait(images.samples, 30)
        click(images.samples)
        wait(path, 30)
        click(path)
    elif how == 'menu':
        click(images.menu_icon)
        click(images.menu_import)
        show_import_options('uncheck')
        add_to_current('uncheck')
        click(images.open_new_file_blue)
        wait(1)
        paste(path+model_name+'.'+format_name)
        type(Key.ENTER)
        click(images.import_and_export_icon_blue)
    elif how == 'new file':
        if exists(images.recent_files, 10):
            click(images.recent_files)
        wait(images.new_file, 10)
        click(images.new_file)
        wait(1)
        paste(path+model_name+'.'+format_name)
        type(Key.ENTER)
        
    display_complete()

def start_set(autotest_name):
    common.HTML_REPORT = ''
    html_report('begin', autotest_name)
    create_report_dir()
    output_dir('delete')
    output_dir('create')
    announce_text('autotest %s'%autotest_name, common.ANN_SWITCH, common.ANN_VOLUME)

def finish_set(*args):
    passed = True
    for arg in args:
        if arg == False:
            passed = False
            break

    test_case_border('start', '--- RESULT ---')
    if passed == True:
        mes = 'Finished, successfully passed!'
        announce_text(mes, common.ANN_SWITCH, common.ANN_VOLUME)
        common.HTML_REPORT += resultAnswer('ok', '')
    else:
        mes = 'Fail, some cases finished unsuccessfully!'
        announce_text(mes, common.ANN_SWITCH, common.ANN_VOLUME)
        common.HTML_REPORT += resultAnswer('fail', mes)
        
    test_case_border('end', '')
    
    html_report('end', '')
    output_dir('delete')

def except_set(tb):
    common.HTML_REPORT += resultAnswer('fail', tb)
    html_report('end', '')
    announce_text('fail, autotest is stopped', common.ANN_SWITCH, common.ANN_VOLUME)