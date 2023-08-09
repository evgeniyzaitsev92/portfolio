# INFORMATION
# This script is intended to show the correlation of resolved bugs/tasks to the created ones per indicated periods.
# The statistics is shown for P1, P2, P3 and all mentioned priorities together and demostrated in 4 histograms.
# For correct script operation the following modules should be installed via 'pip install' if necessary: matplotlib, requests, numpy, sys.

# Minimum number of arguments is 4:
# 1 - Newest sprint number
# 2- Oldest sprint number
# 3 - Starting date of at least one period (sprint)
# 4 - Ending date of at least one period (sprint)

# Maximum number of date pairs is 8.

import requests
import sys
import matplotlib.pyplot as plt
import numpy as np

################
### VIRABLES ###
i_SprintsCount = 0
i_PrioritiesCount = 4

#####################
### Authorization ###
yt_url = '' # CADEX server api here
yt_user = input('Login: ')
yt_pass = input('Password: ')
yt_auth = (yt_user, yt_pass)

##################
### Enter data ###
i_ArgCount = len(sys.argv)
i_SprintNum = 1
b_GetStartDate = True
b_GetEndDate = False

str_Instructions = """
*** Instructions ***

Minimum number of arguments is 4:
1 - Newest sprint number
2- Oldest sprint number
3 - Starting date of at least one period (sprint)
4 - Ending date of at least one period (sprint)

Maximum number of date pairs is 8.
The following modules should be installed via 'pip install' if necessary: matplotlib, requests, numpy, sys.

*** End ***"""

if sys.argv[0] == 'instr':
    print(str_Instructions)
    sys.exit()
else:
    if i_ArgCount < 4 or (i_ArgCount % 2) == 0:
        print(str_Instructions)
        sys.exit()
    else:  
        for i in range(i_ArgCount):
            if i < 3:
                if i == 0:
                    continue
                elif i == 1:
                    i_NewestSprint = int(sys.argv[i])
                elif i == 2:
                    i_OldestSprint = int(sys.argv[i])
            elif i >= 3:
                if b_GetStartDate == True:
                    globals()[f"str_inputDateStart_S{i_SprintNum}"] = sys.argv[i]
                    b_GetStartDate = False
                    b_GetEndDate = True
                elif b_GetEndDate == True:
                    globals()[f"str_inputDateEnd_S{i_SprintNum}"] = sys.argv[i]
                    b_GetStartDate = True
                    b_GetEndDate = False
                    i_SprintsCount += 1
                    i_SprintNum += 1

#############################
### YouTrack bugs request ###
str_bugsQuery = ''

############################## 
### Get number of requests ###
def Get_req_num(type, dateStart, dateEnd, priority):
    count = 0
    dates = dateStart + ' .. ' + dateEnd + ' '
    if type == 'total':
        created_resolved = 'created: '       
    elif type == 'closed':
        created_resolved = 'resolved date: '

    issues_params = {'fields': 'idReadable,customFields(name,value(name))',
                    'query': created_resolved + dates + str_bugsQuery + 'State: -Rejected ' + 'Priority: P' + priority}
    request = requests.get(yt_url + 'issues', auth=yt_auth, params=issues_params)
    if (not request.ok):
        print(request.json(), file=sys.stderr)
        sys.exit(1)

    count = len(request.json())

    return count

##################
### BUGS COUNT ###
## Get lists of TOTAL and CLOSED bugs per sprints
i_Priority = 0
i_Sprint = 0
list_TotalP0 = []; list_TotalP1 = []; list_TotalP2 = []; list_TotalP3 = []
list_ClosedP0 = []; list_ClosedP1 = []; list_ClosedP2 = []; list_ClosedP3 = []

while i_Sprint != i_SprintsCount:
    i_Sprint += 1
    while i_Priority != i_PrioritiesCount:
        globals()[f"list_TotalP{i_Priority}"].append(Get_req_num('total', globals()[f"str_inputDateStart_S{i_Sprint}"], 
                                                                    globals()[f"str_inputDateEnd_S{i_Sprint}"], str(i_Priority)))
        globals()[f"list_ClosedP{i_Priority}"].append(Get_req_num('closed', globals()[f"str_inputDateStart_S{i_Sprint}"], 
                                                                    globals()[f"str_inputDateEnd_S{i_Sprint}"], str(i_Priority)))
        i_Priority += 1
    i_Priority = 0

################
### Graphics ###
## Set sprints numbers
list_SpintsNums = []
if i_NewestSprint > i_OldestSprint:
    list_SpintsNums = [i for i in range(i_NewestSprint, i_OldestSprint-1, -1)]
elif i_NewestSprint < i_OldestSprint:
    list1 = [i for i in range(i_NewestSprint, 0, -1)]
    list2 = [i for i in range(8, i_OldestSprint-1, -1)]
    list_SpintsNums = list1 + list2

## Lines loop
fig = plt.figure(figsize=(16, 10))
i_Priority = 0
i_Sprint = 0
colorClosed = '#808A87'
list_handles = []
list_labels = []

while i_Priority != i_PrioritiesCount:    
    ## Set axis bin names
    globals()[f"X_P{i_Priority}"] = [globals()[f"str_inputDateStart_S{i+1}"][0:4]+'_S'+str(list_SpintsNums[i]) for i in range(i_SprintsCount)]
    
    ## Set lines colors
    if i_Priority==0: colorTotal = 'r'; # red
    elif i_Priority==1: colorTotal = '#FFD700'; # yellow
    elif i_Priority==2: colorTotal = '#228B22'; # green
    elif i_Priority==3: colorTotal = '#4169E1'; # blue

    ## Misc
    plt.subplot(2, 2, i_Priority+1) # plots order
    plt.grid(True) # set grid
    plt.locator_params(axis='y', integer=True) # only integers on Y axes

    ## Define ticks fontsize depending on i_SprintsCount
    if i_SprintsCount == 8: 
        i_TickFontSize = 11
        i_TickFontSizeFirst = 12
    else:
        i_TickFontSize = 12
        i_TickFontSizeFirst = 13

    ## Format ticks
    plt.tick_params(direction='inout', length=8, width=1)
    plt.xticks(fontsize=i_TickFontSize)
    plt.yticks(fontsize=i_TickFontSize)

    ## Format tick name weight for 1st sprint in year
    n = -1 # number of 1st sprint
    # Find tick
    for name in reversed(globals()[f"X_P{i_Priority}"]):
        n += 1
        if name[-1] == '1':
            # Format tick name
            plt.gca().get_xticklabels()[n].set_weight("bold")
            plt.gca().get_xticklabels()[n].set_fontsize(i_TickFontSizeFirst)
            break

    ## Labels locations
    x = np.arange(len(globals()[f"X_P{i_Priority}"])) 
    width = 0.35  # the width of the bars

    ## Bars
    if i_Priority == 0 or i_Priority == 1 or i_Priority == 2:
        rects1 = plt.bar(x - width/2, list(reversed(globals()[f"list_ClosedP{i_Priority}"])), width, color = colorClosed, label='Resolved bugs')
        rects2 = plt.bar(x + width/2, list(reversed(globals()[f"list_TotalP{i_Priority}"])), width, color = colorTotal, label='Created bugs - P'+str(i_Priority))
    elif i_Priority == 3:
        # sum of closed
        zipped_lists_closed = zip(list_ClosedP0, list_ClosedP1, list_ClosedP2)
        list_sum_closed = [x + y + z for (x, y, z) in zipped_lists_closed]
        rects1 = plt.bar(x - width/2, list(reversed(list_sum_closed)), width, color = colorClosed, label='Resolved bugs')

        # sum of total
        zipped_lists_total = zip(list_TotalP0, list_TotalP1, list_TotalP2)
        list_sum_total = [x + y + z for (x, y, z) in zipped_lists_total]
        rects2 = plt.bar(x + width/2, list(reversed(list_sum_total)), width, color = colorTotal, label='Created bugs - P0, P1, P2')

    plt.bar_label(rects1, padding=-0.5, fontsize = 11)
    plt.bar_label(rects2, padding=-0.5, fontsize = 11)

    ## Titles
    if i_Priority == 0 or i_Priority == 1 or i_Priority == 2:
        plt.title('P' + str(i_Priority), fontsize = 20, fontweight='bold', color=colorTotal, bbox=dict(facecolor=colorTotal, 
            alpha=0.25, edgecolor=colorTotal, boxstyle='round', pad=0.1), y=1.0175)
    elif i_Priority == 3:   
        plt.title('All priorities together', fontsize = 20, fontweight='bold', color=colorTotal, bbox=dict(facecolor=colorTotal, 
            alpha=0.25, edgecolor=colorTotal, boxstyle='round', pad=0.1), y=1.0175)

    plt.xlabel('Sprints', labelpad=6, loc="right", fontsize=16, fontweight='bold', style='italic') 
    plt.ylabel('Quantity', labelpad=5, loc="top", fontsize=16, fontweight='bold', style='italic')
    plt.gca().set_xticks(x, reversed(globals()[f"X_P{i_Priority}"]))

    ## Legend info
    Handle, Label = plt.gca().get_legend_handles_labels()
    list_handles.extend(Handle)
    list_labels.extend(Label)

    i_Priority += 1
    
# Show a legend on the plot
list_handles.pop(2); list_handles.pop(3); list_handles.pop(4) # remove duplicates
list_labels = list(dict.fromkeys(list_labels)) # remove duplicates
plt.legend(list_handles, list_labels, loc='upper center', bbox_to_anchor=(-1.2, -0.05, 2.2, -0.15), 
            mode='expand', ncol=5, fontsize=14, edgecolor='black', handletextpad=0.25)

## Saving the plot as an image
fig.suptitle('Correlation of resolved bugs to amount of created ones', fontsize=24, fontweight='bold')
fig.tight_layout()
fig.subplots_adjust(wspace=0.175)
fig.savefig('Correlation of resolved bugs to amount of created ones.png', bbox_inches='tight', dpi=150)

print('--- *** Successfully *** ---')