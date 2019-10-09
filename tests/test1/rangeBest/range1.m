% 0.8M or higher localSize - 256
str1 = 'dataSize';
figure;
select1 = [0,1,2,3,7]
for i = 1:5
  newStr = strcat('../',str1,int2str(select1(i)),'/localSize256.csv');
  data = csvread(newStr)
  threadLoad = data(: , 1);
  gpuTime    = data(:, 2);
  plot(log10(threadLoad), log10(gpuTime));
  hold on  
endfor

title('0.8M or higher Data')
xlabel('Load per Thread')
ylabel('GPU Time in milli seconds')
graphNames = legend('1M','2M','4M','8M','0.8M');

% 0.1M - 0.8M
str2 = 'dataSize';
figure;
select2 = [4,5,6]
for i = 1:3
  newStr = strcat('../',str2,int2str(select2(i)),'/localSize64.csv');
  data = csvread(newStr)
  threadLoad = data(: , 1);
  gpuTime    = data(:, 2);
  plot(log10(threadLoad), log10(gpuTime));
  hold on  
endfor

title('0.1M-0.8M')
xlabel('Load per Thread')
ylabel('GPU Time in milli seconds')
graphNames = legend('0.1M','0.2M','0.4M');

% 2K - 0.1M
str3 = 'dataSize';
figure;
select3 = [8,9,10,11,13,14,15]
for i = 1:7
  newStr = strcat('../',str2,int2str(select3(i)),'/localSize128.csv');
  data = csvread(newStr)
  threadLoad = data(: , 1);
  gpuTime    = data(:, 2);
  plot(log10(threadLoad), log10(gpuTime));
  hold on  
endfor

title('2K-0.1M')
xlabel('Load per Thread')
ylabel('GPU Time in milli seconds')
graphNames = legend('10K','20K','40K','80K','2K','4K','8K');