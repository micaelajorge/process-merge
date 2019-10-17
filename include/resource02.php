<?php

function RemoveLocksTemporarios($UserId) {
    if (empty($UserId)) {
        return;
    }
    global $connect, $userdef;
    $samaccountname = trim($userdef->UserName);
    $SQL = "update ";
    $SQL .= " casequeue ";
    $SQL .= " set ";
    $SQL .= " lockedbyid=0, ";
    $SQL .= " lockedbysamaccountname='', ";
    $SQL .= " lockeddatetime = '' ";
    $SQL .= " where ";
    $SQL .= " lockedbyid = $UserId ";
    $SQL .= " and ";
    $SQL .= " AdHoc = 0 ";
    $QUERY = mysqli_query($connect, $SQL);
}

function IndicePAGINAS_old($Pagina, $TotalRegistros, $Linhas, $destino, $Extend) {
    echo "<div id=\"IndicePAGINAS\">\n";
    $PAGINASPorVez = 10;
    $TotalPAGINAS = bcdiv($TotalRegistros, $Linhas);
    if ($TotalRegistros <= $Linhas) {
        $TotalPAGINAS == 1;
        echo "<span id=\"Pagina\">Anterior</span>\n";
        echo "<span id=\"Pagina\">1</span>\n";
        echo "<span id=\"Pagina\">Pr&oacute;xima</span>\n";
        echo "\n</div>\n";
        return;
    } else {
        if ($TotalPAGINAS == 0) {
            $TotalPAGINAS = 1;
        } else {
            if (bcmod($TotalRegistros, $Linhas) > 0) {
                $TotalPAGINAS++;
            }
        }
    }
    $PosPagina = bcdiv($Pagina, $PAGINASPorVez);
    if (bcmod($Pagina, $PAGINASPorVez) > 0) {
        $PosPagina++;
    }

    if ($TotalPAGINAS > $PAGINASPorVez) {
        if ($Pagina < $PAGINASPorVez + 1) {
            echo "<span id=\"Pagina\">-$PAGINASPorVez</span>\n";
        } else {
            echo "<span id=\"Pagina\"><a href='$destino?Pagina=" . ($Pagina - $PAGINASPorVez) . "&Linhas=$Linhas&$Extend'>-$PAGINASPorVez</a></span>\n";
        }
    }
    $contador = 1;
    if ($Pagina > 1) {
        $anterior = $Pagina - 1;
        echo "<span id=\"Pagina\"><a href='$destino?Pagina=$anterior&Linhas=$Linhas&$Extend'>Anterior</a></span>\n";
    } else {
        echo "<span id=\"Pagina\">Anterior</span>\n";
    }
    while ($contador < $PAGINASPorVez) {
        $IndicePagina = ($contador + ($PosPagina * $PAGINASPorVez) - $PAGINASPorVez);
        if ($IndicePagina >= $TotalPAGINAS) {
            break;
        }
        if ($IndicePagina == $Pagina) {
            echo "<span id=\"Pagina\">$IndicePagina</span>\n";
        } else {
            echo "<span id=\"Pagina\"><a href='$destino?Pagina=" . ($contador + ($PosPagina * $PAGINASPorVez) - $PAGINASPorVez) . "&Linhas=$Linhas&$Extend'>" . ($contador + ($PosPagina * $PAGINASPorVez) - $PAGINASPorVez) . "</a></span>\n";
        }
        $contador++;
        if ($contador >= $PAGINASPorVez) {
            break;
        }
    }
    if ($TotalPAGINAS == ($IndicePAGINAS - $PAGINASPorVez)) {
        echo "<span id=\"Pagina\">$IndicePagina</span>\n";
        echo "<span id=\"Pagina\">Pr&oacute;xima</span>\n";
    } else {
        if (($contador + ($PosPagina * $PAGINASPorVez) - $PAGINASPorVez) == $Pagina) {
            echo "<span id=\"Pagina\">" . ($contador + ($PosPagina * $PAGINASPorVez) - $PAGINASPorVez) . "</span>\n";
        } else {
            echo "<span id=\"Pagina\"><a href='$destino?Pagina=" . ($contador + ($PosPagina * $PAGINASPorVez) - $PAGINASPorVez) . "&Linhas=$Linhas&$Extend'>" . ($contador + ($PosPagina * $PAGINASPorVez) - $PAGINASPorVez) . "</a></span>\n";
        }
        $proxima = $Pagina + 1;
        if ($proxima > $TotalPAGINAS) {
            echo "<span id=\"Pagina\">Pr&oacute;xima</span>\n";
        } else {
            echo "<span id=\"Pagina\"><a href='$destino?Pagina=$proxima&Linhas=$Linhas&$Extend'>Pr&oacute;xima</a></span>\n";
        }
        if (($Pagina + $PAGINASPorVez) < $TotalPAGINAS) {
            echo "<span id=\"Pagina\"><a href='$destino?Pagina=" . ($Pagina + $PAGINASPorVez) . "&Linhas=$Linhas&$Extend'>+$PAGINASPorVez</a></span>\n";
        }
    }
    echo "\n</div>\n";
}

function IndicePAGINAS($paginaAtual, $totalRegistros, $linhasPorPagina, $destino, $Extend) {    
    
    if ($totalRegistros < $linhasPorPagina)
        {
        $totalPaginas = 1;
    } else {
    $modulo = $totalRegistros % $linhasPorPagina;    
    $totalPaginas = (($totalRegistros - $modulo) / $linhasPorPagina) ;    
    $totalPaginas += (($modulo) > 0) ? 1 : 0;        
    }

    $aPaginas = array();
    $pagina = array();

    $ctrInicio["acao"] = "#";
    $ctrInicio["class"] = "";
    $ctrInicio["pagina"] = "inicio";
    
    $ctrAnterior["acao"] = "#";
    $ctrAnterior["class"] = "";
    $ctrAnterior["pagina"] = "anterior";

    $ctrFim["acao"] = "#";
    $ctrFim["class"] = "";
    $ctrFim["pagina"] = "fim";

    $ctrProxima["acao"] = "#";
    $ctrProxima["class"] = "";
    $ctrProxima["pagina"] = "proxima";


    /** 
     *  Verifica se é primeira Página
     */
    if ($paginaAtual != 1) {
        $ctrInicio["acao"] = "$destino?PaginaAtual=1";
        $ctrInicio["class"] = "";
        $paginaDestino = $paginaAtual - 1;
        $ctrAnterior["acao"] = "$destino?PaginaAtual=$paginaDestino";
        $ctrAnterior["class"] = "";
    }
    array_push($aPaginas, $ctrInicio);
    array_push($aPaginas, $ctrAnterior);

    
    for ($i = 0; $i < $totalPaginas; $i++)
    {
        $paginaDestino = $i + 1;
        if ($paginaAtual == $i + 1) {
            $pagina["acao"] = "#";
            $pagina["pagina"] = $i + 1;
            $pagina["class"] = "PAGINAATUAL";
        } else {
            $pagina["acao"] = "$destino?PaginaAtual=$paginaDestino";
            $pagina["pagina"] = $i + 1;            
            $pagina["class"] = "";
        }
        array_push($aPaginas, $pagina);        
    }
        
    /**
     *  Verifica se é ultima página
     */
    if ($paginaAtual != $totalPaginas) {
        $ctrFim["acao"] = "$destino?PaginaAtual=$totalPaginas";
        $paginaDestino = $paginaAtual + 1;
        $ctrProxima["acao"] = "$destino?PaginaAtual=$paginaDestino" ;
    }
    array_push($aPaginas, $ctrProxima);        
    array_push($aPaginas, $ctrFim);
    
    return $aPaginas;
}

function IndicePAGINASXML($Pagina_Atual, $TotalRegistros, $Linhas, $destino, $Extend, $inArray = false) {
    $PAGINASPorVez = 10;
    $TotalPAGINAS = bcdiv($TotalRegistros, $Linhas);
    $a_Paginas = array();


    if ($TotalRegistros <= $Linhas) {
        $TotalPAGINAS = 1;
        if (!$inArray) {
            echo "\t<PAGINAS>\n\t\t<PAGINA>1</PAGINA>\n</PAGINAS>\n";
        } else {
            $paginacao["pagina"] = 1;
            $paginacao["acao"] = "";
            array_push($a_Paginas, $paginacao);
            return $a_Paginas;
        }
        return;
    } else {
        if ($TotalPAGINAS == 0) {
            $TotalPAGINAS = 1;
        }
    }

    $PosPAGINA = bcdiv($Pagina_Atual, $PAGINASPorVez);
    if (bcmod($Pagina_Atual, $PAGINASPorVez) > 0) {
        $PosPAGINA++;
    }

    if ($TotalPAGINAS > $PAGINASPorVez) {
        if ($Pagina_Atual < $PAGINASPorVez + 1) {
            if (!$inArray) {
                echo "\t<PAGINAS>\n\t\t<PAGINA>-$PAGINASPorVez</PAGINA>\n\t</PAGINAS>\n";
            } else {
                $paginacao["pagina"] = "-" . $PAGINASPorVez;
                $paginacao["acao"] = "";
                array_push($a_Paginas, $paginacao);
            }
        } else {
            $IndicePAGINA = $Pagina_Atual - $PAGINASPorVez;
            if (!inArray) {
                echo "<PAGINAS><ACAO>MudaPagina($IndicePAGINA, \"$destino\")</ACAO>\n<PAGINA>-$PAGINASPorVez</PAGINA>\n\t</PAGINAS>\n";
            } else {
                $paginacao["pagina"] = "-" . $PAGINASPorVez;
                $paginacao["acao"] = "MudaPagina($IndicePAGINA, \"$destino\")";
                array_push($a_Paginas, $paginacao);
            }
        }
    }

    $contador = 1;
    if ($Pagina_Atual > 1) {
        $anterior = $Pagina_Atual - 1;
        if (!$inArray) {
            echo "\t<PAGINAS>\n\t\t<ACAO>MudaPagina($anterior, \"$destino\")</ACAO>\n\t\t<PAGINA>Anterior\t\t</PAGINA>\n\t</PAGINAS>\n";
        } else {
            $paginacao["pagina"] = "anterior";
            $paginacao["acao"] = "MudaPagina($anterior, \"$destino\")";
            array_push($a_Paginas, $paginacao);
        }
    } else {
        if (!$inArray) {
            echo "\t<PAGINAS>\n\t\t<PAGINA>Anterior</PAGINA>\n\t</PAGINAS>\n";
        } else {
            $paginacao["pagina"] = "anterior";
            $paginacao["acao"] = "MudaPagina($anterior, \"$destino\")";
            array_push($a_Paginas, $paginacao);
        }
    }


    /**
     *  Indice de Páginas
     */
    while ($contador < $PAGINASPorVez) {
        $IndicePAGINA = ($contador + ($PosPAGINA * $PAGINASPorVez) - $PAGINASPorVez);
        if ($IndicePAGINA > $TotalPAGINAS) {
            break;
        }
        if ($IndicePAGINA == $Pagina_Atual) {
            if (!$inArray) {
                echo "\t<PAGINAS>\n\t\t<PAGINA>$IndicePAGINA</PAGINA>\n\t\t<CLASS>PAGINAATUAL</CLASS>\n\t</PAGINAS>\n";
            } else {
                $paginacao["pagina"] = $IndicePAGINA;
                $paginacao["acao"] = "";
                array_push($a_Paginas, $paginacao);
                $contador++;
                continue;
            }
        } else {
            $IndicePAGINA = ($contador + ($PosPAGINA * $PAGINASPorVez) - $PAGINASPorVez);
            if (!$inArray) {
                echo "\t<PAGINAS>\n\t\t<ACAO>MudaPagina($IndicePAGINA, \"$destino\")</ACAO>\n\t\t<PAGINA>$IndicePAGINA</PAGINA>\n\t</PAGINAS>\n";
            } else {
                $paginacao["pagina"] = $IndicePAGINA;
                $paginacao["acao"] = "MudaPagina($IndicePAGINA, \"$destino\")";
                array_push($a_Paginas, $paginacao);
            }
            $contador++;
            if ($contador >= $PAGINASPorVez) {
                break;
            }
            continue;
        }

        if ($TotalPAGINAS == ($IndicePAGINA - $PAGINASPorVez)) {
            if (!$inArray) {
                echo "\t<PAGINAS>\n\t\t><PAGINA>$IndicePAGINA</PAGINA>\n\t\t<CLASS>PAGINAATUAL</CLASS>\n\t</PAGINAS>\n";
                echo "\t<PAGINAS>\n\t\t><PAGINA>Próxima</PAGINA>\n\t</PAGINAS>\n";
            } else {

                $paginacao["pagina"] = $IndicePAGINA;
                $paginacao["acao"] = "MudaPagina($IndicePAGINA, \"$destino\")";
                $paginacao["class"] = "PAGINAATUAL";
                array_push($a_Paginas, $paginacao);

                $paginacao["pagina"] = "Próxima";
                $paginacao["acao"] = "";
                $paginacao["class"] = "";
                array_push($a_Paginas, $paginacao);
            }
        } else {
            if (($contador + ($PosPAGINA * $PAGINASPorVez) - $PAGINASPorVez) == $Pagina_Atual) {
                $IndicePAGINA = ($contador + ($PosPAGINA * $PAGINASPorVez) - $PAGINASPorVez);
                if (!$inArray) {
                    echo "\t<PAGINAS>\n\t\t<PAGINA>$IndicePAGINA</PAGINA>\n\t\t<CLASS>PAGINAATUAL</CLASS>\n\t</PAGINAS>\n";
                } else {
                    $paginacao["pagina"] = "Próxima";
                    $paginacao["acao"] = "";
                    $paginacao["class"] = "PAGINAATUAL";
                    array_push($a_Paginas, $paginacao);
                }
            } else {
                $IndicePAGINA = ($contador + ($PosPAGINA * $PAGINASPorVez) - $PAGINASPorVez);
                if (!$inArray) {
                    echo "\t<PAGINAS>\n\t\t<ACAO>MudaPagina($IndicePAGINA, \"$destino\")</ACAO>\n\t\t<PAGINA>$IndicePAGINA\t\t</PAGINA>\n\t</PAGINAS>\n";
                } else {
                    $paginacao["pagina"] = IndicePAGINA;
                    $paginacao["acao"] = "MudaPagina($IndicePAGINA, \"$destino\")";
                    $paginacao["class"] = "";
                    array_push($a_Paginas, $paginacao);
                }
            }

            $proxima = $Pagina_Atual + 1;
            if ($proxima > $TotalPAGINAS) {
                if (!$inArray) {
                    echo "\t<PAGINAS>\n\t\t><PAGINA>Próxima</PAGINA>\n\t\n\t\t<CLASS>PAGINAATUAL</CLASS></PAGINAS>\n";
                } else {
                    $paginacao["pagina"] = "Próxima";
                    $paginacao["acao"] = "";
                    $paginacao["class"] = "PAGINAATUAL";
                    array_push($a_Paginas, $paginacao);
                }
            } else {
                if (!$inArray) {
                    echo "\t<PAGINAS>\n\t\t<ACAO>MudaPagina($proxima, \"$destino\")</ACAO>\n\t\t<PAGINA>Próxima\t\t</PAGINA>\n\t</PAGINAS>\n";
                } else {
                    $paginacao["pagina"] = "Próxima";
                    $paginacao["acao"] = "MudaPagina($proxima, \"$destino\")";
                    $paginacao["class"] = "";
                    array_push($a_Paginas, $paginacao);
                }
            }
        }
    }
    if (($Pagina_Atual + $PAGINASPorVez) < $TotalPAGINAS) {
        $IndicePAGINA = $Pagina_Atual + $PAGINASPorVez;
        if (!$inArray) {
            echo "\t<PAGINAS>\n\t\t<ACAO>MudaPagina($IndicePAGINA, \"$destino\")</ACAO>\n\t\t<PAGINA>+$PAGINASPorVez\t\t</PAGINA>\n\t</PAGINAS>\n";
        } else {
            $paginacao["pagina"] = "+$PAGINASPorVez";
            $paginacao["acao"] = "MudaPagina($IndicePAGINA, \"$destino\")";
            $paginacao["class"] = "";
            array_push($a_Paginas, $paginacao);
        }
    }
    return $a_Paginas;
}

function IndicePAGINASXML2($Pagina, $TotalRegistros, $Linhas, $destino, $Extend) {
    $PAGINASPorVez = 10;
    $TotalPAGINAS = bcdiv($TotalRegistros, $Linhas);
    if ($TotalRegistros <= $Linhas) {
        $TotalPAGINAS == 1;
        echo "\t<PAGINAS>\n\t\t<PAGINA>Anterior</PAGINA>\n\t</PAGINAS>\n";
        echo "\t<PAGINAS>\n\t\t<PAGINA>1</PAGINA>\n\t</PAGINAS>\n";
        echo "\t<PAGINAS>\n\t\t<PAGINA>Próxima</PAGINA>\n\t</PAGINAS>\n";
        return;
    } else {
        if ($TotalPAGINAS == 0) {
            $TotalPAGINAS = 1;
        } else {
            if (bcmod($TotalRegistros, $Linhas) > 0) {
                $TotalPAGINAS++;
            }
        }
    }
    $PosPagina = bcdiv($Pagina, $PAGINASPorVez);
    if (bcmod($Pagina, $PAGINASPorVez) > 0) {
        $PosPagina++;
    }

    if ($TotalPAGINAS > $PAGINASPorVez) {
        if ($Pagina < $PAGINASPorVez + 1) {
            echo "<span id=\"Pagina\">-$PAGINASPorVez</span>\n";
        } else {
            $IndicePagina = $Pagina - $PAGINASPorVez;
            echo "<span id=\"Pagina\"><a href='javascript:void();' onclick='MudaPagina($IndicePagina, '$destino')'>-$PAGINASPorVez</a></span>\n";
        }
    }
    $contador = 1;
    if ($Pagina > 1) {
        $anterior = $Pagina - 1;
        echo "\t<PAGINAS>\n\t\t";
        echo "<PAGINA>Anterior</PAGINA>\n\t";
        echo "\t\t<ACAO>MudaPagina($anterior, '$destino')</ACAO>\n";
        echo "\t</PAGINAS>\n";
    } else {
        echo "\t<PAGINAS>\n\t\t";
        echo "<PAGINA>Anterior</PAGINA>\n\t";
        echo "\t<ACAO></ACAO>\n";
        echo "\t\t</PAGINAS>\n";
    }
    while ($contador < $PAGINASPorVez) {
        $IndicePagina = ($contador + ($PosPagina * $PAGINASPorVez) - $PAGINASPorVez);
        if ($IndicePagina >= $TotalPAGINAS) {
            break;
        }
        if ($IndicePagina == $Pagina) {
            echo "\t<PAGINAS>\n\t\t";
            echo "\t\<PAGINA>$IndicePagina</PAGINA>\n";
            echo "\t<CLASS>PaginaAtual</CLASS>\n";
            echo "\t</PAGINAS>\n";
        } else {
            $IndicePagina = ($contador + ($PosPagina * $PAGINASPorVez) - $PAGINASPorVez);
            echo "\t<PAGINAS>\n\t\t";
            echo "<PAGINA>$IndicePagina</PAGINA>\n\t";
            echo "\t\t<ACAO>MudaPagina($IndicePagina, '$destino')</ACAO>\n";
            echo "\t</PAGINAS>\n";
        }
        if ($contador > $PAGINASPorVez) {
            break;
        }
        $contador++;
        if ($TotalPAGINAS == ($IndicePagina - $PAGINASPorVez)) {
            echo "\t<PAGINAS>\n\t\t";
            echo "<PAGINA>$IndicePagina</PAGINA>\n\t";
            echo "\t</PAGINAS>\n";
            echo "\t<PAGINAS>\n\t\t";
            echo "<PAGINA>Proxima</PAGINA>\n";
            echo "\t\t</PAGINAS>\n";
        } else {
            if (($contador + ($PosPagina * $PAGINASPorVez) - $PAGINASPorVez) == $Pagina) {
                $IndicePagina = ($contador + ($PosPagina * $PAGINASPorVez) - $PAGINASPorVez);
                echo "\t<PAGINAS>\n";
                echo "\t\t<PAGINA>$IndicePagina</PAGINA>\n";
                echo "\t\t</PAGINAS>\n";
            } else {
                /*
                  $IndicePagina = ($contador + ($PosPagina * $PAGINASPorVez) - $PAGINASPorVez);
                  echo "\t<PAGINAS>\n";
                  echo "\t\t<PAGINA>$IndicePagina</PAGINA>\n";
                  echo "\t\t<ACAO>MudaPagina($IndicePagina, '$destino')</ACAO>\n";
                  echo "\t</PAGINAS>\n";
                 */
            }
            $proxima = $IndicePagina + 1;
            if ($proxima > $TotalPAGINAS) {
                echo "\t<PAGINAS>\n";
                echo "\t\t<PAGINA>Proxima</PAGINA>\n";
                echo "\t\t</PAGINAS>\n";
            } else {
                if ($proxima == $PAGINASPorVez) {
                    echo "\t<PAGINAS>\n";
                    echo "\t\t<PAGINA>Proxima</PAGINA>\n";
                    echo "\t\t<ACAO>MudaPagina($proxima, '$destino')</ACAO>\n";
                    echo "\t</PAGINAS>\n";
                    if (($Pagina + $PAGINASPorVez) < $TotalPAGINAS) {
                        $IndicePagina = $Pagina + $PAGINASPorVez;
                        echo "\t<PAGINAS>\n";
                        echo "\t\t<PAGINA>+$PAGINASPorVez</PAGINA>\n";
                        echo "\t\t<ACAO>MudaPagina($IndicePagina, '$destino')</ACAO>\n";
                        echo "\t</PAGINAS>\n";
                    }
                }
            }
        }
    }
}

?>