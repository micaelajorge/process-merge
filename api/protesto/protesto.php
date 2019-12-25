<?php


/**
 * @license Apache 2.0
 */

namespace protesto;

/**
 * @OA\Schema(required={"id", "name"})
 */
class Protesto
{     
    /**
     * @OA\Property(example="X00000000000002")
     * @var string
     */
    public $nosso_numero;

    /**
     * @OA\Property(example="C")
     * @var string
     */
    public $tipo_protesto;

    /**
     * @OA\Property(example="CBI")
     * @var string
     */
    public $especie;
    
    /**
     * @OA\Property(example="CBI999991")
     * @var string
     */
    public $num_titulo;
    

    /**
     * @OA\Property(example="")
     * @var string
     */
    public $data_emissao;
    
    /**
     * @OA\Property(example="")
     * @var string
     */
    public $data_vencimento;
    

}
