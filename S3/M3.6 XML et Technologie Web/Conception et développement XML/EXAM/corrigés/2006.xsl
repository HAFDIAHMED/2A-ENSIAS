<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"  version="1.0">
    
       
       
       
       <xsl:template match="/">
  <html>
  	<xsl:apply-templates/>
  </html>
</xsl:template>

<xsl:template match="*">

	  <xsl:value-of select="livre[7]/titre" />	        
	  
 </xsl:template>                   
</xsl:stylesheet>







