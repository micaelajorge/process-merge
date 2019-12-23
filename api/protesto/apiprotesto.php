<?php

use OpenApi\Annotations as OA;

/**
  @OA\Info(title="Search API", version="1.0.0")
 */

/**
   * @OA\Post(
   *     path="/search",
   *     summary="Returns most accurate search result object",
   *     description="Search for an object, if found return it!",
   *     @OA\RequestBody(
   *         description="Client side search object",
   *         required=true,
   *         @OA\MediaType(
   *             mediaType="application/json",                 
   *         @OA\Schema(ref="#/components/schemas/SearchObject")
   *         )
   *     ),
   *     @OA\Response(
   *         response=200,
   *         description="Success",
   *     @OA\Schema(ref="#/components/schemas/SearchResultObject)   
   *     ), 
   *     @OA\Response(
   *         response=404,
   *         description="Could Not Find Resource"
   *     )   
   * )
   */
function CarregaRetorno()
{
    $jDados = "teste";
    echo $jDados;
}
