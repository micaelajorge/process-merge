<?php
/*
  Criação: Marcelo Mosczynski <mmoscz@gmail.com>
  Data Criação 03/04/2019
  Sistema: Process_XAMPP
 */
?>
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">

        <link rel="shortcut icon" href="favicon.png" />

        <title>Marcar Ponto</title>
        <!-- Tell the browser to be responsive to screen width -->
        <meta content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no" name="viewport">
        <!-- Bootstrap 3.3.7 -->
        <link rel="stylesheet" href="vendor/bootstrap/css/bootstrap.min.css">
        <!-- Font Awesome -->
        <link rel="stylesheet" href="vendor/css/font-awesome.min.css">
        <!-- Ionicons -->
        <link rel="stylesheet" href="vendor/css/ionicons.min.css">
        <!-- Theme style -->
        <link rel="stylesheet" href="vendor/css/AdminLTE.min.css">

        <!-- HTML5 Shim and Respond.js IE8 support of HTML5 elements and media queries -->
        <!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
        <!--[if lt IE 9]>
        <script src="https://oss.maxcdn.com/html5shiv/3.7.3/html5shiv.min.js"></script>
        <script src="https://oss.maxcdn.com/respond/1.4.2/respond.min.js"></script>
        <![endif]-->

        <!-- Google Font -->
        <link rel="stylesheet" href="https://fonts.googleapis.com/css?family=Source+Sans+Pro:300,400,600,700,300italic,400italic,600italic">

        <script language="javascript">
            var valorTemporizador = 60;
            var dataServidor = 0;
            var usuarioLogado = 0;
            function jsTemporizador()
            {
                valorTemporizador--;
                $("#temporizador").html("(" + valorTemporizador + ")");
                if (valorTemporizador == 1)
                {
                    jsLogoff();
                } else {
                    setTimeout(jsTemporizador, 1000);
                }
            }
            function jsLogoff()
            {
                url = "logoff";
                $.ajax(
                        {
                            url: url
                        })
                        .done(function (resultado) {
                            alert("Log off");
                            document.location = "marcarponto";
                        })
                        .fail(function () {
                            document.location = "marcarponto";
                        });
            }

            function jsMarcarPonto(textoOcorrencia, tipoOcorrencia)
            {
                
                dadosPontoMarcado = dataServidor.data + " " + usuarioLogado;
                dadosCaso = {
                    Fields: [
                        {
                            "fieldCode": "OCORRENCIA",
                            "fieldValue": textoOcorrencia
                        },
                        {
                            "fieldCode": "TIPOOCORRENCIA",
                            "fieldValue": tipoOcorrencia
                        },
                        {
                            "fieldCode": "PONTO_MARCADO",
                            "fieldValue": dadosPontoMarcado
                        }
                    ]
                };

                url = "api/casestartuniqueprocessa";
                dadosEnvio = {
                    proccode: "CONTROLE_PONTO",
                    stepcode: "ENTRADA_PONTO",
                    casedata: dadosCaso,
                    acao: "processar"
                };
                $("#DivProcessando").show();
                $("#divOpcoesOcorrencias").hide();
                $("#divButtonLogOff").hide();
                $.ajax(
                        {
                            url: url,
                            type: "POST",
                            data: dadosEnvio
                        })
                        .done(function (resultado) {
                            PegaDataHoraRegistro(resultado);
                        });
            }

            function PegadadosUser()
            {
                url = "api/autenticateduser";
                $.ajax(
                        {
                            url: url,
                            type: "POST"
                        }
                ).done(
                        function (retorno) {
                            usuarioLogado = retorno.UserId;
                        }
                );
            }

            function PegaDataHoraRegistro(dadosCaso)
            {
                url = "api/apigetcasedata";                
                dadosEnvio = {
                    caseNum : dadosCaso.CaseNum,
                    fieldCode : "DATA_HORA_OCORRENCIA"
                };
                $.ajax(
                        {
                            url: url,
                            type: "POST",
                            data: dadosEnvio
                        })
                        .done(function (resultado) {
                            var dataLida = moment(resultado.Fields[0].fieldValue);
                            $("#dataHora_confirma").html(dataLida.format("DD/MM/Y h:mm"));
                            $("#divFimAcoes").show();
                            $("#divSelecionarOpcoes").hide();
                            $("#divButtonLogOff").show();
                            valorTemporizador = 10;
                        });

            }

            function PegaDataHora()
            {
                url = "api/datahora";
                $.ajax(
                        {
                            url: url,
                            type: "POST"
                        }
                ).done(
                        function (retorno) {
                            dataServidor = retorno;
                            $("#dataHora").html(retorno.fDataHora);
                        }
                );
            }


        </script>        

    </head>
    <body class="hold-transition">
        <div div="row">
            <!--            <div class="login-logo">
                            <img src="images/logo-souzalima.png" />              
                        </div>-->
            <!-- /.login-logo -->
            <div class="col-lg-4 col-lg-offset-4">
                <div  id="divFimAcoes" style="display:none">
                    <div class="row">
                        <div class="col-lg-12 form-group form-group-lg text-center">
                            <br /><br />
                            Registro de Ponto Efetuado para <b><?= $userdef->UserDesc ?>
                                <h2>
                                Registro: <span id="dataHora_confirma"></span>
                                </h2>
                        </div>
                    </div>                         
                </div>                    
                <div  id="divSelecionarOpcoes">
                    <div class="row">
                        <div class="col-lg-12 form-group form-group-lg text-center">
                            <br /><br />
                            Olá, <b><?= $userdef->UserDesc ?> - <span id="dataHora"></span>
                        </div>
                    </div> 
                    <div style="display:none" id="DivProcessando" class="text-center">
                        <i class="fa fa-4x fa-spin fa-refresh"></i><br />
                        Processando
                    </div>
                    <div id="divOpcoesOcorrencias">
                        <div class="row">
                            <div class="col-lg-12 form-group form-group-lg">
                                <button type="button" class="btn btn-lg btn-info form-control" onclick="jsMarcarPonto('Entrada','ENTRADA')">Inicio Expediente</button>
                            </div>
                        </div>                        
                        <div class="row">
                            <div class="col-lg-12 form-group form-group-lg">
                                <button type="button"  class="btn btn-lg btn-info form-control" onclick="jsMarcarPonto('Saída Almoço','SAIDA_ALMOCO')">Saída Almoço</button>
                            </div>
                        </div>                        
                        <div class="row">
                            <div class="col-lg-12 form-group form-group-lg">
                                <button type="button" class="btn btn-lg btn-info form-control" onclick="jsMarcarPonto('Retorno Almoço','RETORNO_ALMOCO')">Retorno Almoço</button>
                            </div>
                        </div>                        
                        <div class="row">
                            <div class="col-lg-12 form-group form-group-lg">
                                <button type="button" class="btn btn-lg btn-info form-control" onclick="jsMarcarPonto('Saída','SAIDA')">Fim Expediente</button>
                            </div>
                        </div>                        
                    </div>
                </div>
                <div class="row" id="divButtonLogOff">
                    <div class="col-lg-12 form-group form-group-lg">
                        <button onclick="jsLogoff()" type="button" class="btn btn-lg btn-default form-control">Sair <span id="temporizador">10</span></button>
                    </div>
                </div>         
                <div class="row">
                    <div class="col-lg-12">
                        <div class="hidden-xs">
                            <img src="images/logo-agiliza.png" style="float:right;width:50%"></a>
                        </div>
                    </div>
                </div>
            </div>
        </div>
        <!-- /.login-box-body -->
    </div>
    <!-- /.login-box -->
    <div class="visible-xs">
        <div style="position: relative; top: 120px;right:0px;margin-bottom: 20px;margin-right: 20px">
            <div>
                <img src="images/logo-agiliza.png" style="float:right;width:50%"></a>
            </div>
        </div>
    </div>

    <!-- jQuery 3 -->
    <script src="vendor/js/jquery.min.js"></script>
    <!-- moment -->
    <script src="vendor/js/moment.js"></script>
    <!-- Bootstrap 3.3.7 -->
    <script src="vendor/js/bootstrap.min.js"></script>
    <!-- iCheck -->
    <script>
                            function jsInicializacaoPagina()
                            {
                                PegadadosUser();
                                PegaDataHora();
                                jsTemporizador();
                                if ($(window).width() < 640)
                                {
                                    $("#mobileMode").val("1");
                                }
                            }

                            $(document).ready(function () {
                                jsInicializacaoPagina();
                            });
    </script>
</body>
</html>
