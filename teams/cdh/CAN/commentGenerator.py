import csv
import canmatrix.formats
import canmatrix.canmatrix as cm

def toPyObject(infile, **options):
	dbs = {}

	#logger.info("Importing " + infile + " ... ")
	dbs = canmatrix.formats.loadp(infile, **options)
	#logger.info("done\n")

	#logger.info("Exporting " + outfileName + " ... ")

	outdbs = {}
	for name in dbs:
		db = None

		if 'ecus' in options and options['ecus'] is not None:
			ecuList = options['ecus'].split(',')
			db = cm.CanMatrix()
			for ecu in ecuList:
				#logger.info("Copying ECU " + ecu)
				cmcp.copyBUwithFrames(ecu, dbs[name], db)
		if 'frames' in options and options['frames'] is not None:
			frameList = options['frames'].split(',')
			db = cm.CanMatrix()
			for frame in frameList:
				#logger.info("Copying Frame " + frame)
				cmcp.copyFrame(frame, dbs[name], db)
		if db is None:
			db = dbs[name]

		if 'merge' in options and options['merge'] is not None:
			mergeFiles = options['merge'].split(',')
			for database in mergeFiles:
				mergeString = database.split(':')
				dbTempList = canmatrix.formats.loadp(mergeString[0])
				for dbTemp in dbTempList:
					if mergeString.__len__() == 1:
						print ("merge complete: " + mergeString[0])
						for frame in dbTempList[dbTemp].frames:
							cmcp.copyFrame(frame.id, dbTempList[dbTemp], db)
					for mergeOpt in mergeString[1:]:
						if mergeOpt.split('=')[0] == "ecu":
							cmcp.copyBUwithFrames(
								mergeOpt.split('=')[1], dbTempList[dbTemp], db)
						if mergeOpt.split('=')[0] == "frame":
							cmcp.copyFrame(
								mergeOpt.split('=')[1], dbTempList[dbTemp], db)

		if 'renameEcu' in options and options['renameEcu'] is not None:
			renameTuples = options['renameEcu'].split(',')
			for renameTuple in renameTuples:
				old, new = renameTuple.split(':')
				db.renameEcu(old, new)
		if 'deleteEcu' in options and options['deleteEcu'] is not None:
			deleteEcuList = options['deleteEcu'].split(',')
			for ecu in deleteEcuList:
				db.delEcu(ecu)
		if 'renameFrame' in options and options['renameFrame'] is not None:
			renameTuples = options['renameFrame'].split(',')
			for renameTuple in renameTuples:
				old, new = renameTuple.split(':')
				db.renameFrame(old, new)
		if 'deleteFrame' in options and options['deleteFrame'] is not None:
			deleteFrameList = options['deleteFrame'].split(',')
			for frame in deleteFrameList:
				db.delFrame(frame)
		if 'renameSignal' in options and options['renameSignal'] is not None:
			renameTuples = options['renameSignal'].split(',')
			for renameTuple in renameTuples:
				old, new = renameTuple.split(':')
				db.renameSignal(old, new)
		if 'deleteSignal' in options and options['deleteSignal'] is not None:
			deleteSignalList = options['deleteSignal'].split(',')
			for signal in deleteSignalList:
				db.delSignal(signal)

		if 'deleteZeroSignals' in options and options['deleteZeroSignals']:
			db.deleteZeroSignals()

		if 'deleteSignalAttributes' in options and options[
				'deleteSignalAttributes']:
			unwantedAttributes = options['deleteSignalAttributes'].split(',')
			db.delSignalAttributes(unwantedAttributes)

		if 'deleteFrameAttributes' in options and options[
				'deleteFrameAttributes']:
			unwantedAttributes = options['deleteFrameAttributes'].split(',')
			db.delFrameAttributes(unwantedAttributes)

		if 'deleteObsoleteDefines' in options and options[
				'deleteObsoleteDefines']:
			db.deleteObsoleteDefines()

		if 'recalcDLC' in options and options['recalcDLC']:
			db.recalcDLC(options['recalcDLC'])

		#logger.info(name)
		#logger.info("%d Frames found" % (db.frames.__len__()))

		outdbs[name] = db

	#print(outdbs[''].frames._list[0]._name)
	return outdbs['']


def grabDescriptions(filename):
	sigToDesc = {}
	with open(filename) as csvfile:
		reader = csv.DictReader(csvfile)
		for row in reader:
			sigToDesc[row['id']]=row['description']
	return sigToDesc

def addComments(thedb,descriptions):
	database = cm.CanMatrix()
	for frame in next(iter(thedb.values())).frames:
		for signal in frame:
			if signal.name in sigToDesc:
					print("CM_ SG_ " + "%d " % (frame.id+2147483648) +signal.name +" " + '"'+sigToDesc[signal.name]+ '";')

sigToDesc = grabDescriptions('signals.csv')
addComments( canmatrix.formats.loadp("CANDB-MASTER-DS1.dbc"),sigToDesc)