bin_PROGRAMS = maitreya6.bin maitreya6

maitreya6_SOURCES = maitreya6.template

AM_CPPFLAGS = $(WX_CPPFLAGS) -I. -I../base -I../jyotish -I../pdf -I../dialogs -Wall -DSTRICT

maitreya6_bin_SOURCES = ApplicationWindow.cpp ApplicationWindow.h AshtakaVargaView.cpp AshtakaVargaView.h \
	BasicVedicChart.cpp BasicVedicChart.h BasicView.cpp BasicView.h BasicWidget.cpp BasicWidget.h \
	ChildWindow.cpp ChildWindow.h DasaGraphicView.cpp DasaGraphicView.h DasaView.cpp DasaView.h \
	DiagramWidget.cpp DiagramWidget.h Document.cpp Document.h EphemView.cpp EphemView.h GraphicGrid.cpp GraphicGrid.h \
	GraphicView.cpp GraphicView.h guibase.cpp guibase.h MainWindowControl.cpp MainWindowControl.h \
	MenuProvider.cpp MenuProvider.h MultipleChildWindow.cpp MultipleChildWindow.h NonDocTextView.cpp NonDocTextView.h \
	NotebookNodes.cpp NotebookNodes.h NotebookWidget.cpp NotebookWidget.h Painter.cpp Painter.h \
	PartnerView.cpp PartnerView.h PdfBase.cpp PdfBase.h PdfPainter.cpp PdfPainter.h PdfPrinter.cpp PdfPrinter.h \
	PdfWriter.cpp PdfWriter.h SarvatobhadraChart.cpp SarvatobhadraChart.h SbcView.cpp SbcView.h \
	SolarView.cpp SolarView.h SplitterWidget.cpp SplitterWidget.h \
	Statusbar.cpp Statusbar.h TextView.cpp TextView.h TextWidget.cpp TextWidget.h \
	ToolPanel.cpp ToolPanel.h TransitView.cpp TransitView.h TreeWidget.cpp TreeWidget.h \
	UranianView.cpp UranianView.h VargaView.cpp VargaView.h VedicChart.cpp VedicChart.h \
	VedicRasiChart.cpp VedicRasiChart.h WesternChart.cpp WesternChart.h \
	YogaEditor.cpp YogaEditor.h YogaView.cpp YogaView.h 


maitreya6_bin_LDADD = ../jyotish/libjyotish.a ../base/libmaitreya_base.a ../swe/libswe.a \
	../dialogs/libmaitreya_dialogs.a ../pdf/libmaitreya_pdf.a $(WX_LIBS) -lm 

all-am: maitreya6

maitreya6: maitreya6.template
	sed "s|DATAROOTDIR|${datarootdir}|g; s|ABSTOPBUILDDIR|${abs_top_builddir}|g" maitreya6.template > maitreya6
	chmod 755 maitreya6

links: 
	test -d de || mkdir de
	test -L de/maitreya.mo || ln -fs ../../../po/de.gmo de/maitreya.mo
	test -d te || mkdir te
	test -L te/maitreya.mo || ln -fs ../../../po/te.gmo te/maitreya.mo
	test -d pl || mkdir pl
	test -L pl/maitreya.mo || ln -fs ../../../po/pl.gmo pl/maitreya.mo
	test -d ru || mkdir ru
	test -L ru/maitreya.mo || ln -fs ../../../po/ru.gmo ru/maitreya.mo
	test -d it || mkdir it
	test -L it/maitreya.mo || ln -fs ../../../po/it.gmo it/maitreya.mo
	test -d hu || mkdir hu
	test -L hu/maitreya.mo || ln -fs ../../../po/hu.gmo hu/maitreya.mo
	test -d ro || mkdir ro
	test -L ro/maitreya.mo || ln -fs ../../../po/ro.gmo ro/maitreya.mo


pkgdatadir = ${datarootdir}/maitreya6

SUBDIRS = 

EXTRA_DIST = Makefile.win maitreya.rc gui.vcproj maitreya6.template

clean:
	rm -f *.o *~ *.bak maitreya6.bin maitreya6
	rm -fr obj bin


