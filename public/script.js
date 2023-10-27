var chartF;
var chartV;

var eventSource = new EventSource("../data_sse.php");

eventSource.onmessage = function(event) {
  var allData = JSON.parse(event.data);

  // console.log(allData);

  var rate = parseFloat(allData.rate.value1);
  var usage = parseFloat(allData.usage.value2);
  var data = allData.data;
  var value1 = data.map(item => parseFloat(item.value1));
  var value2 = data.map(item => parseFloat(item.value2));
  var reading_time = data.map(item => item.reading_time);
  
  // console.log(debit);
  // console.log(usage);
  // console.log(value1);
  // console.log(value2);
  // console.log(data);

  // Update charts here
  chartF.series[0].setData(value1);
  chartF.xAxis[0].setCategories(reading_time);
  chartV.series[0].setData(value2);
  chartV.xAxis[0].setCategories(reading_time);

  // Data JSON yang ingin ditampilkan
  var jsonData = {data};

  // Ambil elemen dari html
  var tableBody = document.getElementById("table-body");
  var debit = document.getElementById("debit");
  var volume = document.getElementById("volume");

  //replace nilai debit dan volume
  debit.innerHTML = rate;
  volume.innerHTML = usage;

  while (tableBody.firstChild) { // Kosongkan tabel sebelum menambahkan data baru
    tableBody.removeChild(tableBody.firstChild);
  }
  
  jsonData.data.forEach(function(item) {
    var row = document.createElement("tr");
    // Menambahkan class untuk tr
    row.classList.add("bg-white", "border-b", "hover:bg-gray-50");
  
    var readingTimeCell = document.createElement("td");
    readingTimeCell.textContent = item.reading_time;
    // Menambahkan class untuk td
    readingTimeCell.classList.add("px-6", "py-4", "font-medium", "text-gray-900", "whitespace-nowrap");
  
    var debitCell = document.createElement("td");
    debitCell.textContent = item.value1;
    // Menambahkan class untuk td
    debitCell.classList.add("px-6", "py-4", "font-medium", "text-gray-900", "whitespace-nowrap");
  
    var volumeCell = document.createElement("td");
    volumeCell.textContent = item.value2;
    // Menambahkan class untuk td
    volumeCell.classList.add("px-6", "py-4", "font-medium", "text-gray-900", "whitespace-nowrap");
  
    row.appendChild(readingTimeCell);
    row.appendChild(debitCell);
    row.appendChild(volumeCell);
  
    tableBody.appendChild(row);
  });
  
};

document.addEventListener("DOMContentLoaded", function () {
  chartF = new Highcharts.Chart({
    chart: { renderTo: "chart-flowrate" },
    title: { text: "Water Flowrate" },
    series: [{ showInLegend: false, data: [] }],
    plotOptions: {
      line: { 
        dataLabels: { enabled: true }
      },
      series: { color: '#059e8a' }
    },
    xAxis: { type: "datetime", categories: [], reversed: true },
    yAxis: { title: { text: "Liter/Menit" } },
    credits: { enabled: false }
  });

  chartV = new Highcharts.Chart({
    chart: { renderTo: "chart-volume" },
    title: { text: "Water Usage" },
    series: [{ showInLegend: false, data: [] }],
    plotOptions: {
      line: { 
        dataLabels: { enabled: true }
      },
      // series: { color: '#059e8a' }
    },
    xAxis: { type: "datetime", categories: [], reversed: true },
    yAxis: { title: { text: "Liter" } },
    credits: { enabled: false }
  });
});
