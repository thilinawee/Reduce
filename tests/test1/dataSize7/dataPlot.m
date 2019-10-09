str = 'localSize';
figure;
for i = 1:10
  newStr = strcat(str,int2str(pow2(i)),'.csv');
  data = csvread(newStr)
  threadLoad = data(: , 1);
  gpuTime    = data(:, 2);
  plot(log10(threadLoad), log10(gpuTime));
  hold on  
endfor
title('GPU Time vs Load per Thread for 0.8M elements')
xlabel('Load per Thread')
ylabel('GPU Time in milli seconds')
graphNames = legend('2','4','8','16','32','64','128','256','512','1024');
set(graphNames,'title','Local Sizes')
print("graph7.png")
print("../graphs/graph7.png")