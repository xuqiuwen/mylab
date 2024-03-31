option = {
    tooltip: {
        trigger: 'axis'
    },
    legend: {
        data: ['缓存命中率', '平均每个请求的I/O次数']
    },
    xAxis: {
        type: 'category',
        data: ['1024', '2048', '3072', '4096', '5120', '6144', '7168', '8192', '9216', '10240', '11264', '12288']
    },
    yAxis: [
        {
            type: 'value',
            name: '缓存命中率',
            min: 0,
            max: 1,
            position: 'left',
            axisLabel: {
                formatter: '{value} '
            }
        },
        {
            type: 'value',
            name: '平均每个请求的I/O次数',
            min: 0,
            position: 'right',
            axisLabel: {
                formatter: '{value} 次'
            }
        }
    ],
    series: [
        {
            name: '命中率',
            type: 'line',
            smooth: true,
            data: [0.33913, 0.419138, 0.471168, 0.51088, 0.543668, 0.571488, 0.59579, 0.617492, 0.636998, 0.65506, 0.67137, 0.686448] // 命中率数据
        },
        {
            name: 'I/O次数',
            type: 'line',
            yAxisIndex: 1,
            smooth: true,
            data: [0.983494, 0.86347, 0.784848, 0.724644, 0.674952, 0.632482, 0.59526, 0.562018, 0.531892, 0.503978, 0.478862, 0.455374] // I/O次数数据
        }
    ]
};
