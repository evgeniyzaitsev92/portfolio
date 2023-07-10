# INFORMATION
# This script is intended to show the correlation of resolved bugs/tasks to the created ones per indicated periods.
# The statistics is shown for P1, P2, P3 and all mentioned priorities together and demostrated in 4 histograms.
# For correct script operation the following modules should be installed via 'pip install' if necessary: matplotlib, requests, time, datetime, numpy, sys.

# Minimum number of arguments is 4:
# 1 - Newest sprint number
# 2- Oldest sprint number
# 3 - Starting date of at least one period (sprint)
# 4 - Ending date of at least one period (sprint)

# Maximum number of date pairs is 8.

import matplotlib as mp
import requests
import time
import datetime
import matplotlib.pyplot as plt
import numpy as np
import sys

################
### VIRABLES ###
i_SprintsCount = 0
i_PrioritiesCount = 3
i_WorkingDay = 480 # one working day in minutes, 60 * 8 hours

#####################
### Authorization ###
yt_url = '' # CADEX server api here
yt_user = input('Login: ')
yt_pass = input('Password: ')
yt_auth = (yt_user, yt_pass)

###################
### Enter dates ###
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
The following modules should be installed via 'pip install' if necessary: matplotlib, requests, time, datetime, numpy, sys.

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

def Calc_FlowEff(dateStart, dateEnd, priority):
    #################### 
    ### Get requests ###
    list_Issues = []
    dates = dateStart + ' .. ' + dateEnd + ' '
    issues_params = {'fields': 'idReadable,customFields(name,value(name))',
                    'query': 'resolved date: ' + dates + str_bugsQuery + 'State: -Rejected '  + 'Priority: P' + priority}

    request = requests.get(yt_url + 'issues', auth=yt_auth, params=issues_params)
    if (not request.ok):
        print(request.json(), file=sys.stderr)
        sys.exit(1)

    list_Issues.extend(request.json())

    ######################################### 
    ### Calculate average flow efficiency ###
    list_FlowEffs = []
    activities_params = {'fields': 'added(name),removed(name),timestamp,field(name),author(fullName)',
                        'categories': 'CustomFieldCategory,IssueCreatedCategory,IssueResolvedCategory'}

    for issue in list_Issues:
        b_ResolvedDateActFound = False
        b_SpentTimeActFound = False

        ## Get activities
        requestActivities = requests.get(yt_url + 'issues/{}/activities'.format(issue['idReadable']), auth=yt_auth, params=activities_params)
        activities_chunk = requestActivities.json()

        ## Go through each list of activities
        for item in reversed(activities_chunk):
            i_WhenWasCreated = activities_chunk[0]['timestamp'] # date of creation

            if b_ResolvedDateActFound == False:
                # Find Resolved Date activity
                if item['field']['name'] == 'resolved date':
                    i_WhenWasResolved = item['timestamp'] # when was resolved
                    b_ResolvedDateActFound = True # found

            # Find Spent time
            if b_SpentTimeActFound == False:
                if item['field']['name'] == 'Spent Time':
                    i_SpentTime = item['added'] # total time spent on task in MINUTES
                    b_SpentTimeActFound = True # found

            # When Spent time and Resolved date found, CALCULATE FLOW EFFICIENCY
            if b_ResolvedDateActFound == True and b_SpentTimeActFound == True:
                # Date of creation
                dt_Create = datetime.datetime.fromtimestamp(i_WhenWasCreated / 1e3)
                d_createdDate = datetime.date(dt_Create.year, dt_Create.month, dt_Create.day)

                # Date of close
                dt_Resolved = datetime.datetime.fromtimestamp(i_WhenWasResolved / 1e3)
                d_resolvedDate = datetime.date(dt_Resolved.year, dt_Resolved.month, dt_Resolved.day)

                i_NumOfBusDays = np.busday_count(d_createdDate, d_resolvedDate) # consider only working days
                # If i_NumOfBusDays = 0, divide by (dt_Resolved - dt_Create)
                # If i_NumOfBusDays > 0, divide by Working days
                if i_NumOfBusDays > 0:
                    f64_FlowEff = (i_SpentTime/(i_NumOfBusDays*i_WorkingDay))*100 # CALCULATE FLOW EFFICIENCY (FE)
                elif i_NumOfBusDays == 0:
                    length = dt_Resolved - dt_Create # period from Created to Resolved
                    f64_FlowEff = (i_SpentTime/(length.seconds/60))*100 # CALCULATE FLOW EFFICIENCY (FE)
                list_FlowEffs.append(f64_FlowEff) # add one Flow Eff
                break
            
    if len(list_FlowEffs) > 0:
        f64_AvFlowEff = round((sum(list_FlowEffs) / len(list_FlowEffs)), 1)
    else:
        f64_AvFlowEff = np.nan

    return f64_AvFlowEff

## Get lists Flow Efficiencies per sprints
i_Priority = 0
i_Sprint = 0
list_FlowEffP0 = []; list_FlowEffP1 = []; list_FlowEffP2 = []
while i_Sprint != i_SprintsCount:
    i_Sprint += 1
    while i_Priority != i_PrioritiesCount:
        globals()[f"list_FlowEffP{i_Priority}"].append(Calc_FlowEff(globals()[f"str_inputDateStart_S{i_Sprint}"], 
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
fig = plt.figure(figsize=(2.375*i_SprintsCount, 8))

## Misc
plt.grid(axis='y') # set grid
plt.locator_params(axis='y', integer=True) # only integers on Y axes

## Set axis bin names
X_P = [globals()[f"str_inputDateStart_S{i+1}"][0:4]+'_S'+str(list_SpintsNums[i]) for i in range(i_SprintsCount)]

## Set limits
plt.xlim(-0.5, float(i_SprintsCount)-0.5)

## Format ticks
plt.tick_params(direction='inout', length=8, width=1)
plt.xticks(fontsize=14)
plt.yticks(fontsize=14)

## Format tick name weight for 1st sprint in year
n = -1 # number of 1st sprint
# Find tick
for name in reversed(X_P):
    n += 1
    if name[-1] == '1':
        # Format tick name
        plt.gca().get_xticklabels()[n].set_weight("bold")
        break

## Labels
x = np.arange(len(X_P)) 
width = 0.25  # the width of the bars
plt.gca().set_xticks(x, reversed(X_P)) # set tasks names

## Bars
rectsP0 = plt.bar((x-width)-(width/2), list(reversed(list_FlowEffP0)), width, align='edge',  color = 'r', label='P0')
rectsP1 = plt.bar(x-(width/2), list(reversed(list_FlowEffP1)), width, align='edge', color = '#FFD700', label='P1')
rectsP2 = plt.bar((x+width)-(width/2), list(reversed(list_FlowEffP2)), width, align='edge', color = '#228B22', label='P2')
plt.bar_label(rectsP0, padding=3.0, fontsize = 12)
plt.bar_label(rectsP1, padding=3.0, fontsize = 12)
plt.bar_label(rectsP2, padding=3.0, fontsize = 12)

## Titles
plt.xlabel('Sprints', fontsize=20, fontweight='bold', style='italic') 
plt.ylabel('Flow efficiency, %', fontsize=20, fontweight='bold', style='italic')
fig.suptitle('Average flow efficiency', fontsize=26, fontweight='bold')

## Legend
plt.legend(loc='upper center', bbox_to_anchor=(0.0, 0.05, 1.0, -0.15), 
           ncol=3, fontsize=14, edgecolor='black', handletextpad=0.25)

## Saving the plot as an image
fig.tight_layout()
fig.subplots_adjust(wspace=0.1825, hspace=0.3)
fig.savefig('Average flow efficiency.png', bbox_inches='tight', dpi=150)

print('--- *** Successfully *** ---')