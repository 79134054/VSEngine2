/*
find the path "Microsoft Visual Studio 9.0\Common7\Packages\Debugger" autoexp.dat 
copy them to [Visualizer] 

VSEngine2::VSString{
		preview
		(
			[$c.m_pBuffer,s]
		)
}
VSEngine2::VSName{
		preview
		(

			[$c.m_String]

		)
}
VSEngine2::VSUsedName{
		preview
		(
			[$c.m_pName.m_pObject]
		)
}
VSEngine2::VSBitArray{
	children   
	(   
		#array   
		(   

			expr : ($e.m_pBuffer[$i / 32] >> ($i % 32)),     
			size : ($e.m_uiSize * 32) 
		)  : (bool)($e & 1) 
	)   

	preview   
	(    
		#(    
			"[m_uiCurUse is", 
			($e.m_uiSize * 32) ,
			"](",  
			#array   
				(   
					expr : ($e.m_pBuffer[$i / 32] >> ($i % 32)), 
					size : ($e.m_uiSize * 32)
				): (bool)($e & 1),    
			")"  
		)   
 

	)
}
VSEngine2::VSArray<*,*>{
	children   
		(   
		#array   
				(   
				expr : $c.m_pBuffer[$i],     
				size : $c.m_uiCurUse   
			   )   
	   )   

	   preview   
	   (    
		#(    
			   "[m_uiCurUse is", 
			   $c.m_uiCurUse ,
			   "]",
			   "[m_uiBufferNum is",    
			   $c.m_uiBufferNum ,   
			   "](",    

		#array   
			   (   
				expr : $c.m_pBuffer[$i],     
				size : $c.m_uiCurUse   
			   ),    
			   ")"  
		)   
	   )  

}
VSEngine2::VSArrayOrder<*,*>{
	children   
		(   
			#array   
				(   
				expr : $c.m_pBuffer[$i],     
				size : $c.m_uiCurUse   
				)   
		)   

		preview   
		(    
		 #(    
				"[m_uiCurUse is", 
				$c.m_uiCurUse ,
				"]",
				"[m_uiBufferNum is",    
				$c.m_uiBufferNum ,   
				"](",    

			#array   
				(   
				expr : $c.m_pBuffer[$i],     
				size : $c.m_uiCurUse   
				),    
				")"  
		 )   
		)  

}

VSEngine2::VSMap<*,*>{
	children   
	(   
		#array   
		(   
		expr : $c.m_pBuffer[$i],     
		size : $c.m_uiCurUse   
		)   
	)   

	preview   
	(    
		#(    
			"[m_uiCurUse is", 
			$c.m_uiCurUse ,
			"]",
			"[m_uiBufferNum is",    
			$c.m_uiBufferNum ,   
			"](",    

			#array   
			(   
			expr : $c.m_pBuffer[$i],     
			size : $c.m_uiCurUse   
			),    
			")"  
		)   
	)  

}
VSEngine2::VSMapOrder<*,*>{
	children   
	(   
		#array   
		(   
		expr : $c.m_pBuffer[$i],     
		size : $c.m_uiCurUse   
		)   
	)   

	preview   
	(    
		#(    
			"[m_uiCurUse is", 
			$c.m_uiCurUse ,
			"]",
			"[m_uiBufferNum is",    
			$c.m_uiBufferNum ,   
			"](",    

			#array   
			(   
			expr : $c.m_pBuffer[$i],     
			size : $c.m_uiCurUse   
			),    
			")"  
		)   
	)  

}
*/