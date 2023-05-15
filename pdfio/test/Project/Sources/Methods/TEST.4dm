//%attributes = {}
$path:=Get 4D folder:C485(Current resources folder:K5:16)+"test.pdf"

$status:=PDFio Get metadata($path)

SET TEXT TO PASTEBOARD:C523(JSON Stringify:C1217($status; *))