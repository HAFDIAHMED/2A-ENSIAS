<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"  version="1.0">
       
       <xsl:template match="/">
  <html><body>

<xsl:for-each select="livre/section">

	    <tr> <xsl:value-of select="titre" /> </tr>
	     <tr> <xsl:value-of select="figure/image" /> </tr>
	    
	    <xsl:for-each select="section">
	   <tr><tr> <tr> <xsl:value-of select="titre" /> </tr></tr></tr>
	   <tr> <xsl:value-of select="figure/image" /> </tr>
	    </xsl:for-each>
	  
	</xsl:for-each>
</body>
</html>
</xsl:template>
</xsl:stylesheet>







