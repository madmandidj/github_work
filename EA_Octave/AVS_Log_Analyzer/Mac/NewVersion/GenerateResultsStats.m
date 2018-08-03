function resultsStats = GenerateResultsStats(_resultsWithTriggerID, _noiseTotalNum, _distInNoiseNum, _trigInDistNum)  numOfCols = columns(_resultsWithTriggerID);  numOfRows = rows(_resultsWithTriggerID);  resultsStats = cell(numOfRows,1);  noiseCol = numOfCols - 2;  distCol = numOfCols - 1;  trigCol = numOfCols;  curRow = 1;%%  Total stats vars  totalRecognizedNum = 0;  totalRecognizedPercentage = 0;  totalFailedNum = 0;  totalFailedPercentage = 0;%% Noise Group stats vars  noiseRecognizedNum = cell(_noiseTotalNum,1);  noiseRecognizedNum(:,1) = 0;  noiseRecognizedPercentage = cell(_noiseTotalNum,1);  noiseFailedNum = cell(_noiseTotalNum,1);  noiseFailedNum(:,1) = 0;  noiseFailedPercentage = cell(_noiseTotalNum,1);      %% Distance Group stats vars  distRecognizedNum = cell(_distInNoiseNum,1);  distRecognizedPercentage = cell(_distInNoiseNum,1);  distFailedNum = cell(_distInNoiseNum,1);  distFailedPercentage = cell(_distInNoiseNum,1);%% Trigger Group stats vars  triggerRecognizedNum = cell(_trigInDistNum,1);  triggerRecognizedPercentage = cell(_trigInDistNum,1);  triggerFailedNum = cell(_trigInDistNum,1);  triggerFailedPercentage = cell(_trigInDistNum,1);    curStatsRow = 1;    %%  Total stats  for curTrig = 1 : numOfRows    if (iscellstr(_resultsWithTriggerID(curTrig, 1)))      ++totalRecognizedNum;    else        ++totalFailedNum;    endif  endfor  totalRecognizedPercentage = 100 * (totalRecognizedNum / numOfRows);  totalFailedPercentage = 100 * (totalFailedNum / numOfRows);  resultsStats(curStatsRow,1) = cellstr("Total Statistics");  resultsStats(curStatsRow,2) = cellstr("");  ++curStatsRow;  resultsStats(curStatsRow, 1) = cellstr("Total Recognized Num");  resultsStats(curStatsRow, 2) = cellstr(num2str(totalRecognizedNum));  ++curStatsRow;  resultsStats(curStatsRow, 1) = cellstr("Total Recognized Percentage");  resultsStats(curStatsRow, 2) = cellstr(sprintf("%s%s",num2str(totalRecognizedPercentage), "%"));  ++curStatsRow;  resultsStats(curStatsRow, 1) = cellstr("Total Failed Num");  resultsStats(curStatsRow, 2) = cellstr(num2str(totalFailedNum));  ++curStatsRow;  resultsStats(curStatsRow, 1) = cellstr("Total Failed Percentage");  resultsStats(curStatsRow, 2) = cellstr(sprintf("%s%s",num2str(totalFailedPercentage), "%"));  ++curStatsRow;    %%Two Blank Rows  resultsStats(curStatsRow, 1:2) = {"",""};  ++curStatsRow;  resultsStats(curStatsRow, 1:2) = {"",""};  ++curStatsRow;    %%  Noise group stats  curTrigger = 0;  for curNoise = 1:_noiseTotalNum    for curDist = 1:_distInNoiseNum      for curTrig = 1:_trigInDistNum        ++curTrigger;        newCount = cell2mat(noiseRecognizedNum(curNoise,1));        ++newCount;        if (iscellstr(_resultsWithTriggerID(curTrigger, 1)))          noiseRecognizedNum(curNoise,1) = newCount;        else            noiseFailedNum(curNoise,1) = newCount;        endif      endfor    endfor  endfor  for curNoise = 1:_noiseTotalNum    recognizedForNoise = cell2mat(noiseRecognizedNum(curNoise,1));    noiseRecognizedPercentage(curNoise,1) = 100 * (recognizedForNoise / (_trigInDistNum * _distInNoiseNum));    failedForNoise = cell2mat(noiseFailedNum(curNoise,1));    noiseFailedPercentage(curNoise,1) = 100 * (failedForNoise / (_trigInDistNum * _distInNoiseNum));  endfor      resultsStats(curStatsRow,1) = cellstr("Noise Group Statistics");  resultsStats(curStatsRow,2) = cellstr("");  ++curStatsRow;  for curNoise = 1 : _noiseTotalNum    recognizedForNoise = cell2mat(noiseRecognizedNum(curNoise,1));    resultsStats(curStatsRow, 1) = cellstr(sprintf("%s %d Recognized Num", "Noise Group", curNoise));    resultsStats(curStatsRow, 2) = cellstr(num2str(recognizedForNoise));    ++curStatsRow;    resultsStats(curStatsRow, 1) = cellstr(sprintf("%s %d Recognized Percentage", "Noise Group", curNoise));    resultsStats(curStatsRow, 2) = cellstr(sprintf("%d%s",cell2mat(noiseRecognizedPercentage(curNoise,1)), "%"));    ++curStatsRow;  endfor  
endfunction
