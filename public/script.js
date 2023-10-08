var chartF;
var chartV;

var eventSource = new EventSource("../data_sse.php");

eventSource.onmessage = function(event) {
  var data = JSON.parse(event.data);

  console.log(data);

  // var value1 = data.map(item => item.value1);
  // var value2 = data.map(item => item.value2);
  // var reading_time = data.map(item => item.reading_time);

  var value1 = data.map(item => parseFloat(item.value1));
  var value2 = data.map(item => parseFloat(item.value2));
  var reading_time = data.map(item => item.reading_time);
  
  // console.log(value1);
  // console.log(value2);
  // console.log(reading_time);

  // Update charts here
  chartF.series[0].setData(value1);
  chartF.xAxis[0].setCategories(reading_time);
  chartV.series[0].setData(value2);
  chartV.xAxis[0].setCategories(reading_time); 
};

document.addEventListener("DOMContentLoaded", function () {
  chartF = new Highcharts.Chart({
    chart: { renderTo: "chart-flowrate" },
    title: { text: "Water Flowrate" },
    series: [{ data: [] }],
    plotOptions: {
      line: { 
        dataLabels: { enabled: true }
      },
      series: { color: '#059e8a' }
    },
    xAxis: { type: "datetime", categories: [], reversed: true },
    yAxis: { title: { text: "Liter/Menit" } },
  });

  chartV = new Highcharts.Chart({
    chart: { renderTo: "chart-volume" },
    title: { text: "Pemakaian" },
    series: [{ data: [] }],
    plotOptions: {
      line: { 
        dataLabels: { enabled: true }
      },
      // series: { color: '#059e8a' }
    },
    xAxis: { type: "datetime", categories: [], reversed: true },
    yAxis: { title: { text: "Liter" } },
  });
});