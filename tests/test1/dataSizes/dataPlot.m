str = 'localSize';
figure;
for i = 1:10
  newStr = strcat(str,int2str(pow2(i)),'.csv');
  data = csvread(newStr)
  threadLoad = data(: , 1);
  gpuTime    = data(:, 2);
  plot(log(threadLoad), log(gpuTime))
  hold on  
endfor
title('GPU Time vs Load per Thread for 8M elements')
xlabel('Load per Thread')
ylabel('GPU Time in milli seconds')
graphNames = legend('2','4','8','16','32','64','128','256','512','1024');
set(graphNames,'title','Local Sizes')