<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="7.5.0">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="2" name="Route2" color="1" fill="3" visible="no" active="no"/>
<layer number="3" name="Route3" color="4" fill="3" visible="no" active="no"/>
<layer number="4" name="Route4" color="1" fill="4" visible="no" active="no"/>
<layer number="5" name="Route5" color="4" fill="4" visible="no" active="no"/>
<layer number="6" name="Route6" color="1" fill="8" visible="no" active="no"/>
<layer number="7" name="Route7" color="4" fill="8" visible="no" active="no"/>
<layer number="8" name="Route8" color="1" fill="2" visible="no" active="no"/>
<layer number="9" name="Route9" color="4" fill="2" visible="no" active="no"/>
<layer number="10" name="Route10" color="1" fill="7" visible="no" active="no"/>
<layer number="11" name="Route11" color="4" fill="7" visible="no" active="no"/>
<layer number="12" name="Route12" color="1" fill="5" visible="no" active="no"/>
<layer number="13" name="Route13" color="4" fill="5" visible="no" active="no"/>
<layer number="14" name="Route14" color="1" fill="6" visible="no" active="no"/>
<layer number="15" name="Route15" color="4" fill="6" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="I_O_connectors">
<packages>
<package name="SLW-104-01-X-D">
<description>Low Profile Contact Socket Strip, Double Row Through Hole.</description>
<wire x1="-5.08" y1="2.4765" x2="5.08" y2="2.4765" width="0.127" layer="51"/>
<wire x1="5.08" y1="2.4765" x2="5.08" y2="-2.4765" width="0.127" layer="51"/>
<wire x1="5.08" y1="-2.4765" x2="-5.08" y2="-2.4765" width="0.127" layer="51"/>
<wire x1="-5.08" y1="-2.4765" x2="-5.08" y2="2.4765" width="0.127" layer="51"/>
<wire x1="-5.588" y1="2.9845" x2="5.588" y2="2.9845" width="0.127" layer="21"/>
<wire x1="5.588" y1="2.9845" x2="5.588" y2="-2.9845" width="0.127" layer="21"/>
<wire x1="5.588" y1="-2.9845" x2="-5.588" y2="-2.9845" width="0.127" layer="21"/>
<wire x1="-5.588" y1="-2.9845" x2="-5.588" y2="2.9845" width="0.127" layer="21"/>
<pad name="1" x="3.81" y="-1.27" drill="0.8636" diameter="1.2954"/>
<pad name="2" x="3.81" y="1.27" drill="0.8636" diameter="1.2954"/>
<pad name="3" x="1.27" y="-1.27" drill="0.8636" diameter="1.2954"/>
<pad name="4" x="1.27" y="1.27" drill="0.8636" diameter="1.2954"/>
<pad name="5" x="-1.27" y="-1.27" drill="0.8636" diameter="1.2954"/>
<pad name="6" x="-1.27" y="1.27" drill="0.8636" diameter="1.2954"/>
<pad name="7" x="-3.81" y="-1.27" drill="0.8636" diameter="1.2954"/>
<pad name="8" x="-3.81" y="1.27" drill="0.8636" diameter="1.2954"/>
<text x="-2.54" y="3.302" size="1.016" layer="25">&gt;NAME</text>
<text x="-2.54" y="-4.318" size="1.016" layer="27">&gt;VALUE</text>
<text x="3.556" y="-4.572" size="1.27" layer="21">1</text>
<text x="3.556" y="3.302" size="1.27" layer="21">2</text>
</package>
<package name="KEYSTONE_W/NICHROME_5016">
<smd name="P$1" x="0" y="0" dx="2.6" dy="4.2" layer="1" rot="R90"/>
<smd name="P$2" x="-2.54" y="0" dx="1.27" dy="1.5" layer="1"/>
</package>
</packages>
<symbols>
<symbol name="SLW-104-01-X-D">
<wire x1="-7.62" y1="5.08" x2="5.08" y2="5.08" width="0.254" layer="94"/>
<wire x1="5.08" y1="5.08" x2="5.08" y2="-7.62" width="0.254" layer="94"/>
<wire x1="5.08" y1="-7.62" x2="-7.62" y2="-7.62" width="0.254" layer="94"/>
<wire x1="-7.62" y1="-7.62" x2="-7.62" y2="5.08" width="0.254" layer="94"/>
<text x="-7.62" y="5.842" size="1.27" layer="95">&gt;NAME</text>
<text x="-7.62" y="-9.652" size="1.27" layer="96">&gt;VALUE</text>
<pin name="1" x="-12.7" y="2.54" length="middle"/>
<pin name="2" x="10.16" y="2.54" length="middle" rot="R180"/>
<pin name="3" x="-12.7" y="0" length="middle"/>
<pin name="4" x="10.16" y="0" length="middle" rot="R180"/>
<pin name="5" x="-12.7" y="-2.54" length="middle"/>
<pin name="6" x="10.16" y="-2.54" length="middle" rot="R180"/>
<pin name="7" x="-12.7" y="-5.08" length="middle"/>
<pin name="8" x="10.16" y="-5.08" length="middle" rot="R180"/>
</symbol>
<symbol name="1PIN_I/O">
<pin name="P$1" x="0" y="-5.08" visible="off" length="middle" rot="R90"/>
<wire x1="-2.54" y1="0" x2="2.54" y2="0" width="0.254" layer="94"/>
<wire x1="2.54" y1="0" x2="2.54" y2="5.08" width="0.254" layer="94"/>
<wire x1="2.54" y1="5.08" x2="-2.54" y2="5.08" width="0.254" layer="94"/>
<wire x1="-2.54" y1="5.08" x2="-2.54" y2="0" width="0.254" layer="94"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="SLW-104-01-X-D" prefix="J" uservalue="yes">
<description>Low Profile Contact Socket Strip, Double Row Through Hole.</description>
<gates>
<gate name="G$1" symbol="SLW-104-01-X-D" x="0" y="0"/>
</gates>
<devices>
<device name="" package="SLW-104-01-X-D">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="6" pad="6"/>
<connect gate="G$1" pin="7" pad="7"/>
<connect gate="G$1" pin="8" pad="8"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="KEYSTONE_W/NICHROME_5016">
<gates>
<gate name="G$1" symbol="1PIN_I/O" x="0" y="-2.54"/>
</gates>
<devices>
<device name="" package="KEYSTONE_W/NICHROME_5016">
<connects>
<connect gate="G$1" pin="P$1" pad="P$1"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="J1" library="I_O_connectors" deviceset="SLW-104-01-X-D" device=""/>
<part name="U$14" library="I_O_connectors" deviceset="KEYSTONE_W/NICHROME_5016" device=""/>
<part name="U$16" library="I_O_connectors" deviceset="KEYSTONE_W/NICHROME_5016" device=""/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="J1" gate="G$1" x="55.88" y="35.56"/>
<instance part="U$14" gate="G$1" x="71.12" y="35.56"/>
<instance part="U$16" gate="G$1" x="78.74" y="38.1"/>
</instances>
<busses>
</busses>
<nets>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
