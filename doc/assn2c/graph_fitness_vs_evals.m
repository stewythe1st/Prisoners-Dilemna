clear;
close all;

% Global variables
file1(1,:) = '../../log/default.csv      ';
file2(1,:) = '../../log/default_best.csv ';
outputname(1,:) = './images/default.png  ';
file1(2,:) = '../../log/high_mut.csv     ';
file2(2,:) = '../../log/high_mut_best.csv';
outputname(2,:) = './images/high_mut.png ';
file1(3,:) = '../../log/no_mut.csv       ';
file2(3,:) = '../../log/no_mut_best.csv  ';
outputname(3,:) = './images/no_mut.png   ';


for i=1:size(file1,1)
    
    clear data1 data2 max min max_idx min_idx;

    % Read in data
    data1 = csvread(file1(i,:),1,0);
    data2 = csvread(file2(i,:),1,0);

    % Narrow down to only data from the best run
    [max,max_idx] = max(data1);
    [min,min_idx] = min(data1);
    best_run = data1(max_idx(4),:);
    best_run = best_run(1);
    idxs = find(data1(:,1)==best_run);
    best_data = data1(idxs,:);

    fig = figure('rend','painters','pos',[100 100 1000 400]); 

    % Plot data
    subplot(1,2,1);
    hold on;
    plot(best_data(:,2),best_data(:,4),'-','color','blue');
    plot(best_data(:,2),best_data(:,3),'--','color','blue');
    plot(best_data(:,2),best_data(:,6),'-','color','red');
    plot(best_data(:,2),best_data(:,5),'--','color','red');
    hold off;
    ylim([0,3]);
    xlabel('Run');
    ylabel('Fitness');
    title('Fitness vs Evals');
    legend('Best Composite', 'Average Composite', 'Best Absolute', 'Average Absolute','Location','Southeast');

    % Parse boxplot data
    idxs = find(data2(:,1)==best_run);
    box_data = data2(idxs,:);
    subplot(1,2,2);
    boxplot([box_data(:,3),box_data(:,4)],'labels', {'Composite','Absolute'});
    ylim([1.75,3]);
    title('Final Population Fitness Distribution');
    ylabel('Fitness');
    
    % Save image
    fig.PaperPositionMode = 'auto';
    print(outputname(i,:),'-dpng','-r0')
    
end
