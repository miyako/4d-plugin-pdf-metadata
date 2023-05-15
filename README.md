# 4d-plugin-pdf-metadata
Read and write PDF metadata with [PDFio](https://github.com/michaelrsweet/pdfio)

## Modifications

- [x] Support Unicode file paths on Windows https://github.com/michaelrsweet/pdfio/issues/18

## Note

This library does not seem to support file modification; either read an existing PDF or create a new one.

## Example

```4d
$path:=Get 4D folder(Current resources folder)+"test.pdf"

$status:=PDFio Get metadata($path)
```

```json
{
	"success": true,
	"meta": {
		"version": "1.4",
		"permission": {
			"print": true,
			"modify": true,
			"copy": true,
			"annotate": true,
			"forms": true,
			"reading": true,
			"assemble": true,
			"printHigh": true
		},
		"encryption": "NONE",
		"pageCount": 1,
		"name": "/Users/miyako/Documents/GitHub/4d-plugin-pdf-metadata/pdfio/test/Resources/test.pdf",
		"creationDate": "1970-01-01T00:00:00Z"
	}
}
```
