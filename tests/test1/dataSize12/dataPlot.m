str = 'localSize';
figure;
for i = 1:6
  newStr = strcat(str,int2str(pow2(i)),'.csv');
  data = csvread(newStr)
  threadLoad = data(: , 1);
  gpuTime    = data(:, 2);
  plot(log10(threadLoad), log10(gpuTime));
  hold on  
endfor
title('GPU Time vs Load per Thread for 1K elements')
xlabel('Load per Thread')
ylabel('GPU Time in milli seconds')
graphNames = legend('2','4','8','16','32','64');
set(graphNames,'title','Local Sizes')
print("graph12.png")
print("../graphs/graph12.png")