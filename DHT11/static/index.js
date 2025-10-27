async function getNow() {
  const res = await fetch("/api/now");
  const data = await res.json();

  if (!data || data.length < 2) {
    document.getElementById("now").innerText = "❌ 센서 데이터 읽기 실패";
    return;
  }

  document.getElementById("now").innerText =
    `현재 온도: ${data[0]}°C / 습도: ${data[1]}%`;

  // 새로 데이터 삽입 후 테이블/그래프 갱신
  updateTable();
  updateChart();
}

async function updateTable() {
  const res = await fetch("/api/record");
  const rows = await res.json();
  const tbody = document.getElementById("table-body");
  tbody.innerHTML = "";

  rows.forEach(r => {
    const tr = document.createElement("tr");
    tr.innerHTML = `
      <td>${r[0]}</td>
      <td>${r[1]}</td>
      <td>${r[2]}</td>
      <td>${r[3]}</td>
    `;
    tbody.appendChild(tr);
  });
}

async function updateChart() {
  const res = await fetch("/api/record");
  const rows = await res.json();

  const labels = rows.map(r => r[3]);
  const temps = rows.map(r => r[1]);
  const hums = rows.map(r => r[2]);

  const ctx = document.getElementById("graph").getContext("2d");

  if (window.myChart) window.myChart.destroy();

  window.myChart = new Chart(ctx, {
    type: "line",
    data: {
      labels: labels,
      datasets: [
        {
          label: "온도 (°C)",
          data: temps,
          borderColor: "red",
          backgroundColor: "rgba(255,0,0,0.1)",
          tension: 0.3,
        },
        {
          label: "습도 (%)",
          data: hums,
          borderColor: "blue",
          backgroundColor: "rgba(0,0,255,0.1)",
          tension: 0.3,
        }
      ]
    },
    options: {
      responsive: true,
      plugins: {
        legend: { position: 'top' },
        title: { display: true, text: "최근 온습도 변화 그래프" }
      },
      scales: {
        y: { beginAtZero: true }
      }
    }
  });
}

// 페이지 처음 로드 시 테이블과 그래프 초기화
window.onload = () => {
  updateTable();
  updateChart();
};
