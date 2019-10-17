<?php
/*
 * Classe para descobrir o tipo MIME de um arquivo
 * Copyright (c) 2003 Sebastião Farias Júnior
 * Belém/PA
 * E-mail: overond@yahoo.com
 * Licença: GNU General Public License (GPL)
 * Modo de usar:
 * $oMime = new mime("arq.ice");
 * echo $oMime->getMime();
 */
class mime{
	var $pExt;
	var $pMime;
	
	function setExt($b){
		$this->pExt = strtolower(GetType($dot=strrpos($b,"."))=="integer" ? substr($b,$dot) : "");
	}
	function setMime($c){
		$this->pMime = $c;
	}
	function getExt(){
		return $this->pExt;
	}
	function getMime(){
		return $this->pMime;
	}
	function set_mime(){
		switch($this->getExt()){
			case '.ez':
			  $this->setMime("application/andrew-inset");
			break;
			case '.hqx':
				$this->setMime("application/mac-binhex40");
			break;
			case '.cpt':
				$this->setMime("application/mac-compactpro");
			break;
			case '.doc':
				$this->setMime("application/msword");
			break;
			case '.bin':
			case '.dms':
			case '.lha':
			case '.lzh':
			case '.exe':
			case '.class':
			case '.so':
			case '.dll':
				$this->setMime("application/octet-stream");
			break;
			case '.oda':
				$this->setMime("application/oda");
			break;
			case '.pdf':
				$this->setMime("application/pdf");
			break;
			case '.ai':
			case '.eps':
			case '.ps':
				$this->setMime("application/postscript");
			break;
			case '.smi':
			case '.smil':
				$this->setMime("application/smil");
			break;
			case '.mif':
				$this->setMime("application/vnd.mif");
			break;
			case '.xls':
				$this->setMime("application/vnd.ms-excel");
			break;
			case '.ppt':
				$this->setMime("application/vnd.ms-powerpoint");
			break;
			case '.wbxml':
				$this->setMime("application/vnd.wap.wbxml");
			break;
			case '.wmlc':
				$this->setMime("application/vnd.wap.wmlc");
			break;
			case '.wmlsc':
				$this->setMime("application/vnd.wap.wmlscriptc");
			break;
			case '.bcpio':
				$this->setMime("application/x-bcpio");
			break;
			case '.vcd':
				$this->setMime("application/x-cdlink");
			break;
			case '.pgn':
				$this->setMime("application/x-chess-pgn");
			break;
			case '.cpio':
				$this->setMime("application/x-cpio");
			break;
			case '.csh':
				$this->setMime("application/x-csh");
			break;
			case '.dcr':
			case '.dir':
			case '.dxr':
				$this->setMime("application/x-director");
			break;
			case '.dvi':
				$this->setMime("application/x-dvi");
			break;
			case '.spl':
				$this->setMime("application/x-futuresplash");
			break;
			case '.gtar':
				$this->setMime("application/x-gtar");
			break;
			case '.gzip':
				$this->setMime("application/x-gzip");
			break;
			case '.hdf':
				$this->setMime("application/x-hdf");
			break;
			case '.js':
				$this->setMime("application/x-javascript");
			break;
			case '.skp':
			case '.skd':
			case '.skt':
			case '.skm':
				$this->setMime("application/x-koan");
			break;
			case '.latex':
				$this->setMime("application/x-latex");
			break;
			case '.nc':
			case '.cdf':
				$this->setMime("application/x-netcdf");
			break;
			case '.sh':
				$this->setMime("application/x-sh");
			break;
			case '.shar':
				$this->setMime("application/x-shar");
			break;
			case '.swf':
				$this->setMime("application/x-shockwave-flash");
			break;
			case '.sit':
				$this->setMime("application/x-stuffit");
			break;
			case '.sv4cpio':
				$this->setMime("application/x-sv4cpio");
			break;
			case '.sv4crc':
				$this->setMime("application/x-sv4crc");
			break;
			case '.tar':
				$this->setMime("application/x-tar");
			break;
			case '.tcl':
				$this->setMime("application/x-tcl");
			break;
			case '.tex':
				$this->setMime("application/x-tex");
			break;
			case '.texinfo':
			case '.texi':
				$this->setMime("application/x-texinfo");
			break;
			case '.t':
			case '.tr':
			case '.roff':
				$this->setMime("application/x-troff");
			break;
			case '.man':
				$this->setMime("application/x-troff-man");
			break;
			case '.me':
				$this->setMime("application/x-troff-me");
			break;
			case '.ms':
				$this->setMime("application/x-troff-ms");
			break;
			case '.ustar':
				$this->setMime("application/x-ustar");
			break;
			case '.src':
				$this->setMime("application/x-wais-source");
			break;
			case '.xhtml':
			case '.xht':
				$this->setMime("application/xhtml+xml");
			break;
			case '.zip':
				$this->setMime("application/zip");
			break;
			case '.au':
			case '.snd':
				$this->setMime("audio/basic");
			break;
			case '.mid':
			case '.midi':
			case '.kar':
				$this->setMime("audio/midi");
			break;
			case '.mpga':
			case '.mp2':
			case '.mp3':
				$this->setMime("audio/mpeg");
			break;
			case '.aif':
			case '.aiff':
			case '.aifc':
				$this->setMime("audio/x-aiff");
			break;
			case '.m3u':
				$this->setMime("audio/x-mpegurl");
			break;
			case '.ram':
			case '.rm':
				$this->setMime("audio/x-pn-realaudio");
			break;
			case '.rpm':
				$this->setMime("audio/x-pn-realaudio-plugin");
			break;
			case '.ra':
				$this->setMime("audio/x-realaudio");
			break;
			case '.wav':
				$this->setMime("audio/x-wav");
			break;
			case '.pdb':
				$this->setMime("chemical/x-pdb");
			break;
			case '.xyz':
				$this->setMime("chemical/x-xyz");
			break;
			case '.bmp':
				$this->setMime("image/bmp");
			break;
			case '.gif':
				$this->setMime("image/gif");
			break;
			case '.ief':
				$this->setMime("image/ief");
			break;
			case '.jpeg':
			case '.jpg':
			case '.jpe':
				$this->setMime("image/jpeg");
			break;
			case '.png':
				$this->setMime("image/png");
			break;
			case '.tiff':
			case '.tif':
				$this->setMime("image/tiff");
			break;
			case '.djvu':
			case '.djv':
				$this->setMime("image/vnd.djvu");
			break;
			case '.wbmp':
				$this->setMime("image/vnd.wap.wbmp");
			break;
			case '.ras':
				$this->setMime("image/x-cmu-raster");
			break;
			case '.pnm':
				$this->setMime("image/x-portable-anymap");
			break;
			case '.pbm':
				$this->setMime("image/x-portable-bitmap");
			break;
			case '.pgm':
				$this->setMime("image/x-portable-graymap");
			break;
			case '.ppm':
				$this->setMime("image/x-portable-pixmap");
			break;
			case '.rgb':
				$this->setMime("image/x-rgb");
			break;
			case '.xbm':
				$this->setMime("image/x-xbitmap");
			break;
			case '.xpm':
				$this->setMime("image/x-xpixmap");
			break;
			case '.xwd':
				$this->setMime("image/x-xwindowdump");
			break;
			case '.igs':
			case '.iges':
				$this->setMime("model/iges");
			break;
			case '.msh':
			case '.mesh':
			case '.silo':
				$this->setMime("model/mesh");
			break;
			case '.wrl':
			case '.vrml':
				$this->setMime("model/vrml");
			break;
			case '.css':
				$this->setMime("text/css");
			break;
			case '.html':
			case '.htm':
				$this->setMime("text/html");
			break;
			case '.asc':
			case '.txt':
				$this->setMime("text/plain");
			break;
			case '.rtx':
				$this->setMime("text/richtext");
			break;
			case '.rtf':
				$this->setMime("text/rtf");
			break;
			case '.sgml':
			case '.sgm':
				$this->setMime("text/sgml");
			break;
			case '.tsv':
				$this->setMime("text/tab-separated-values");
			break;
			case '.wml':
				$this->setMime("text/vnd.wap.wml");
			break;
			case '.wmls':
				$this->setMime("text/vnd.wap.wmlscript");
			break;
			case '.etx':
				$this->setMime("text/x-setext");
			break;
			case '.xml':
			case '.xsl':
				$this->setMime("text/xml");
			break;
			case '.mpeg':
			case '.mpg':
			case '.mpe':
				$this->setMime("video/mpeg");
			break;
			case '.qt':
			case '.mov':
				$this->setMime("video/quicktime");
			break;
			case '.mxu':
				$this->setMime("video/vnd.mpegurl");
			break;
			case '.avi':
				$this->setMime("video/x-msvideo");
			break;
			case '.movie':
				$this->setMime("video/x-sgi-movie");
			break;
			case '.ice':
				$this->setMime("x-conference/x-cooltalk");
			break;
			default:
			  $this->setMime("application/octet-stream");
		    }
		
		}
		function mime($e){
			$this->setExt($e);
			$this->set_mime();
		}
}
?>