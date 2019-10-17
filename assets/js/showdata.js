// //Versao 1.0.0 /Versao
// JavaScript Document
function showdate() {
var months = new Array(
  "Janeiro",
  "Fevereiro",
  "Março",
  "Abril",
  "Maio",
  "Junho",
  "Julho",
  "Agosto",
  "Setembro",
  "Outubro",
  "Novembro",
  "Dezembro");
var days = new Array(
  "Domingo",
  "Segunda-feira",
  "Terça-feira",
  "Quarta-feira",
  "Quinta-feira",
  "Sexta-feira",
  "Sábado");
var today = new Date();
var weekDay = today.getDay();
var mday = today.getDate();
var mon = today.getMonth();
var year = today.getYear();
var todayValue = "" + days[weekDay] + ", " + mday + " de " + months[mon] + " de " + year;
document.write(todayValue);
}
