/* 
 * pres-switch.com.br
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function isObject(value) {
  return Object(value) === value;
}

function Template(larquivo) {
    var that = this;
    /**
     * Pega os campos do Template
     * @returns {undefined}
     */
    var indentifyVars = function () {
        var patern = /{(.*?)}/;
        var match;
        var completo = that.master;
        that.fields = [];
        while ((match = patern.exec(completo)))
        {
            if (!that.fields[match[1]]) // Inclui o Field se n�o incluido
            {
                that.fields.push(match[0]); // Adiciona o Campo a Lista de Campos
                Object.defineProperty(Template, match[1], {set: function (valor) {
                        that[match[0]] = valor;
                    }});
            }
            completo = completo.replace(new RegExp(match[0], 'g'), "");
            //completo = completo.replace(match[0], ""); // Retira a Referencia
        }
    };


    /**
     * Pega os Blocos do Template
     * @returns {undefined}
     */
    var indentifyBlocks = function () {
        that.blockQueue = []; // Fila de Blocos
        that.blockList = []; // Lista de Blocos
        that.blockParents = []; // Parentesco entre Blocos
        that.blockParse = [];
        that.blockList["."] = ""; // Seta valor do bloco;
        that.blocoAtual = "."; // Seta o Bloco Principal        
        that.blockQueue.push("."); // Adiciona a Raiz na fila
        that.hasContent = [];

        var lines = that.master.split("\n");
        // Checking for minified HTML
        if (1 === lines.length) {
            that.master = that.master.replace('-->', "-->\n");
            lines = that.master.split("\n");
        }

        /**
         * Percorre as linhas a procura do Inicio e fim dos Blocos
         * @type Number
         */
        for (var i = 0; i < lines.length; i++) {
            var linha = lines[i];
            /**
             * 
             * @type RegExp Padrao de inicio de bloco
             */
            var paternInicio = /<!--\s*BEGIN\s+(.*?)\s*-->/sm;
            /**
             * 
             * @type RegExp Padrao de fim de bloco
             */
            var paternFim = /<!--\s*END\s+(.*?)\s*-->/sm;

            /**
             *  Procura por inicio de um bloco
             */
            var match = paternInicio.exec(linha);
            if (match !== null)
            {
                that.blockQueue.push(match[1]); // Coloca na Fila de Blocos Aninhados a ser preenchidos
                var conteudo = that.blockList[that.blocoAtual];
                that.blockList[that.blocoAtual] = conteudo + "\n{BLOCK_" + match[1] + "}\n"; // Seta o bloco como Variavel
                var parentesco = [that.blocoAtual, match[1]];
                that["BLOCK_" + match[1]] = "";
                that.fields.push("{BLOCK_" + match[1] + "}");
                that.blockParents.push(parentesco);
                that.blocoAtual = match[1];
                that.blockList[match[1]] = ""; // Inicia o Bloco
                that.blockParse[match[1]] = ""; // Inicia a Variavel de Blocos;
            } else
            {
                var match = paternFim.exec(linha); // Procura se � o fim de um bloco
                if (match === null)
                {
                    var conteudo = that.blockList[that.blocoAtual];
                    that.blockList[that.blocoAtual] = conteudo + linha; // Adiciona o Conteudo do Bloco
                } else
                {
                    that.blockQueue.pop(); // Remove o ultimo bloco
                    that.blocoAtual = that.blockQueue[that.blockQueue.length - 1]; // Pega o bloco anterior
                }
            }
        }
    };

    /**
     * Faz um parse do Bloco Unit�riamente e limpa o Parse do Bloco
     * 
     * @param {type} nomeBloco
     * @returns {Template.blockParse}
     */
    this.blockParse = function (nomeBloco) {
        var parsedOld = that.blockParse[nomeBloco]; // Guarda o valor do Bloco Parseado anteriormente
        that.makeBlock(nomeBloco); // Faz o Parse do Bloco
        var parsedData = that.blockParse[nomeBloco]; // Pega o valor para o conteudo do Bloco parseado
        that.blockParse[nomeBloco] = parsedOld; // Retorna o Valor do Bloco Parseado anteriormente
        return parsedData;
    };

    // Limpa o Bloco ap�s ter sido preenchido
    clearBlock = function (nomeBloco) {
        for (var i = 0; i < that.blockParents.length; i++)
        {
            if (that.blockParents[i][0] === nomeBloco)
            {
                //that["BLOCK_" + nomeBloco] = "";
                var indexFound = that.hasContent.indexOf(that.blockParents[i][1]);
                that[that.blockParents[i][1]] = "";
                if (indexFound > -1)
                {
                    that.blockParse[that.blockParents[i][1]] = "";
                    that.hasContent = that.hasContent.splice(indexFound);
                }
            }
        }

    };


    /**
     * Inicializa a gera��o do BLoco na estrutura do HTML
     * 
     * @param {type} nomeBloco
     * @returns {undefined}
     */
    this.block = function (nomeBloco) {

        //        Procura o Bloco Pai do Bloco a ser preenchido
        for (var i = 0; i < that.blockParents.length; i++)
        {
            if (that.blockParents[i][1] === nomeBloco) {
                var parentBlock = that.blockParents[i][0];
            }
        }

        // Insere o Bloco como um campo na lista de campos
        if (that.fields.indexOf("{BLOCK_" + nomeBloco + "}") === -1)
        {
            that.fields.push("{BLOCK_" + nomeBloco + "}");
            that["BLOCK_" + nomeBloco] = "";
        }

        // coloca na listagem de Blocos Preenchidos
        if (that.hasContent.indexOf(nomeBloco) === -1)
        {
            that.hasContent.push(nomeBloco);
        }

        that["BLOCK_" + nomeBloco] = that["BLOCK_" + nomeBloco] + makeBlock(nomeBloco);

        // declara que o Bloco PAI tem um filho com conteudo
        if (parentBlock !== ".")
        {
            if (that.hasContent.indexOf(parentBlock) === -1)
            {
                that.hasContent.push(parentBlock);
            }
        }

        // Limpa 
        clearBlock(nomeBloco);
    };

    /**
     * Limpa o valor do campo
     * 
     * @param {type} nomeCampo
     * @returns {undefined}
     */
    clear = function (nomeCampo) {
        that["nomeCampo"] = "";
    };

    /**
     * Cria��o do HTML do BLOCO
     * 
     * @param {type} nomeBloco
     * @returns {that.blockList}
     */
    makeBlock = function (nomeBloco) {
        var toParse = that.blockList[nomeBloco];
        if (toParse === undefined)
        {
            return "";
        }
        for (var i = 0; i < that.fields.length; i++)
        {
            var field = that.fields[i];
            var fieldValue = that[field.replace("{", "").replace("}", "")];
            //var fieldValue = (fieldValue) !== "" ? "" : that[field.replace("{", "").replace("}", "")];            
            toParse = toParse.replace(field, fieldValue);
        }
        that.blockParse[nomeBloco] = that.blockParse[nomeBloco] + toParse; // Guarda o Valor ap�s ter feito o parse do Bloco
        return toParse;
    };

    /**
     * Gera o HTML para ser enviado para o Browser
     * 
     * @returns {string}
     */
    this.parse = function ()
    {
        var vAtual = "";
        that.blockParse["."] = that.blockList["."];

        for (var i = 0; i < that.fields.length; i++)
        {
            var field = that.fields[i];
            var prop = field.replace("{", "").replace("}", "");
            that.blockParse["."] = that.blockParse["."].replace(field, that[prop]);
        }

        vAtual = that.blockParse["."];
        clearBlock(".");
        return vAtual;
    };

    // Preenche os campos com os elementos do Objeto
    this.parseJSONObject = function (objRead) {
        aData = Object.entries(objRead); // Converte o Objeto em array        
        if (aData.length === 1)
        {
            dados = Object.entries(aData[0]);
            chave = dados[0][1];
            valor = dados[1][1];
            that.parseJSONBlock(chave, valor);
        } else {
            for (var i = 0; i < aData.length; i++)
            {
                chave = aData[0];
                valor = aData[1];
                that[chave] = valor;
            }
        }
    }
    ;

    /**
     * Preenche o bloco com os elementos filhos
     * 
     * @param {type} blockName
     * @param {type} aData
     * @returns {undefined}
     */
    this.parseJSONBlock = function (blockName, aData) {
        for (var i = 0; i < aData.length; i++)
        {
            item = aData[i];            
            if (isObject(item))
            {
                if (isObject(item[1]))
                {
                    // Se � um objeto, tenta criar como Bloco filho
                    that.parseJSONBlock(item[0], Object.entries(item[1]));
                } else {
                    // Preenche os campos com as propriedades do Objeto
                    that.parseJSONObject(item[1]);
                }
            } else
            {
                that[item[0]] = item[1];
            }
            that["contadorElementos"] = i; // Preeche o contador de Elementos;                                 
            that.block(blockName); // Gera o Bloco para o Elemento Pai do JSON
        }
    };

    /**
     * 
     * @param {type} obj
     * @returns {undefined}
     */
    this.parseJSON = function (objRead) {

        // Transforma o obj em array
        aData = Object.entries(objRead);

        for (var i = 0; i < aData.length; i++) {
            chave = aData[i][0];
            valor = aData[i][1];
            if (valor instanceof Object)
            {
                that.parseJSONBlock(chave, Object.entries(valor)); // Quando encontra um Objeto JSON ou Array, come�a a fazer a cadeia de blocos
            } else
            {
                that[chave] = valor; // Preenche os valores dos Campos
            }
        }
        ;
    };



    /**
     * 
     * @param {type} urlData
     * @returns {undefined}
     */
    this.getDataRest = function (urlData) {
        $.get(urlData, function (dataRead) {
            getData(dataRead);
        });
    };

    /**
     * 
     * @param {type} dataRead
     * @returns {undefined}
     */
    this.getData = function (dataRead) {
        var objReceived = JSON.parse(dataRead);
        that.parseJSON(objReceived);
    };

    /**
     * 
     * @param {type} obj
     * @returns {undefined}
     */
    this.showInfo = function (obj) {
        if (that.infoTemplate !== "undefined")
        {
            var fieldsTX = "<b>Campos</b><br />";
            var blocksTX = "<b>Blocos</b><br />";
            that.fields.forEach(function (item, index) {
                var fieldName = item.replace("{", "").replace("}", "");
                if (item.indexOf("BLOCK_") === -1)
                {
                    fieldsTX = fieldsTX + fieldName + "<br />";
                } else
                {
                    blocksTX = blocksTX + fieldName.replace("BLOCK_", "") + "<br />";
                }
            });
        }

        $("#" + obj).html("<p>" + fieldsTX + "</p>" + "<p>" + blocksTX + "</p>");
    };

    /**
     * 
     * @param {type} arquivo
     * @returns {undefined}
     */
    var LoadFile = function (arquivo) {
        that.master = getFile(arquivo);
        indentifyVars();
        indentifyBlocks();
    };

    /**
     * Pega o Arquivo de forma sincrona
     * @param {type} arquivo
     * @returns {data}
     */
    var getFile = function (arquivo) {
        var retorno;
        $.ajax({
            type: "GET",
            url: arquivo,
            async: false,
            success: function (data) {
                retorno = data;
            }
        });

        return retorno;
    };

    /**
     * 
     * @param {type} larquivo - Arquivo de Template
     * @returns {template}
     */
    LoadFile(larquivo);
}