# Autor: Prof. Alejandro T. Bello Ruiz
# Curso: INF239 Sistemas Operativos
# Laboratorio 5

from tkinter import *
import tkinter as tk
import tkinter.filedialog as fd
import tkinter.messagebox as mb
from tkinter import font as tkfont
from tkinter import Scrollbar as sb


class App(tk.Tk):
    def __init__(self):
        super().__init__()
        self.fname = None
        self.title("Visor File System FAT")         
        self.canvas = tk.Canvas(self, width=450, height=480, bg="white")         
        btn_file = tk.Button(self, text="Choose file system image",command=self.choose_file)         
        btn_showBoot = tk.Button(self, text="Boot Sector",command=self.show_bootsector)
        btn_showFat = tk.Button(self, text="File Allocation Table",command=self.show_fat)
        btn_showRootDir = tk.Button(self, text="Root directory",command=self.show_rootdir)                  
        btn_file.grid(row=0, column=0, columnspan=2, padx=60,pady=20 )
                
        btn_showBoot.grid(row=1, column=0)
        btn_showFat.grid(row=1, column=1)
        btn_showRootDir.grid(row=2, column=0, columnspan=2,padx=60,pady=20,sticky='nswe')
        self.canvas.grid(row=3, column=0,rowspan=3, columnspan=2)
        

        # ATRIBUTOS EXTRA 
        #self.bytes_por_sector = 0 
        #self.sectores_por_cluster = 0
        #self.reserved_sectors = 0
        
    def choose_file(self):
        filetypes = (("File sistem images", "*.img"),("All files", "*"))
        filename = fd.askopenfilename(title="Open file", initialdir="/home/alejandro/SourceCode/Python", filetypes=filetypes)
        self.fname = filename
            
            
    def show_bootsector(self):
        if self.fname == None:
            self.show_error1()
        else:    
            with open(self.fname,mode='rb') as file:
                type_fat = self.determine_type(file)
                file.seek(0)
                if(type_fat == '12' or type_fat == '16' or type_fat == '32'):
                    self.canvas.delete("all")
                    bold_font = tkfont.Font(family="Helvetica", size=12, weight="bold")

                    self.canvas.create_text(180,20, text="Boot Sector",anchor=tk.W)
                    j1=file.read(1)
                    i1=int.from_bytes(j1,byteorder='little')
                    j2=file.read(1)
                    i2=int.from_bytes(j2,byteorder='little')
                    j3=file.read(1)
                    i3=int.from_bytes(j3,byteorder='little')
                    self.canvas.create_text(30,40, text="Code to jump the bootstrap code: "+format(i1,'X')+" "+format(i2,'X')+" "+format(i3,'X'),anchor=tk.W)
                    
                    fabricante=file.read(8)
                    texto=fabricante.decode('utf-8')
                    self.canvas.create_text(30,60, text='Oem ID - Name of the formatting OS: '+texto,anchor=tk.W)
                    
                    bytesps=file.read(2)
                    i=int.from_bytes(bytesps,byteorder='little')
                    #self.bytes_por_sector = i 
                    self.canvas.create_text(30,80, text='Bytes per Sector: '+str(i),anchor=tk.W, font= bold_font)
                    
                    sectorespc=file.read(1)
                    i=int.from_bytes(sectorespc,byteorder='little')
                    #self.sectores_por_cluster = i
                    self.canvas.create_text(30,100, text='Sectors per Cluster: '+str(i),anchor=tk.W, font= bold_font)
                    
                    sectoresr = file.read(2)
                    i=int.from_bytes(sectoresr,byteorder='little')
                    #self.reserved_sectors = i
                    self.canvas.create_text(30,120, text='Reserved sectors: '+str(i),anchor=tk.W, font= bold_font)
                    
                    nfats=file.read(1)
                    i=int.from_bytes(nfats,byteorder='little')
                    self.canvas.create_text(30,140, text='Number of FAT copies: '+str(i),anchor=tk.W)
                    
                    # muestra el numero de entradas de directorio de 32 bytes en el directorio raiz
                    entradasRaizdir = file.read(2)
                    i=int.from_bytes(entradasRaizdir,byteorder='little')
                    self.canvas.create_text(30,160, text='Number of possible root entries: '+str(i),anchor=tk.W, font= bold_font)
                    
                    sectorest=file.read(2)
                    i=int.from_bytes(sectorest,byteorder='little')                  
                    self.canvas.create_text(30,180, text='Small number of sectors: '+str(i),anchor=tk.W, font= bold_font)
                                    
                    descriptorm=file.read(1)
                    i=int.from_bytes(descriptorm,byteorder='little')
                    self.canvas.create_text(30,200, text='Media Descriptor: '+format(i,'X'),anchor=tk.W)
                                    
                    sectorespf=file.read(2)
                    i=int.from_bytes(sectorespf,byteorder='little')
                    self.canvas.create_text(30,220, text='Sectors per FAT: '+str(i),anchor=tk.W, font= bold_font)
                    
                    sectorespt=file.read(2)
                    i=int.from_bytes(sectorespt,byteorder='little')
                    self.canvas.create_text(30,240, text='Sectors per Track: '+str(i),anchor=tk.W)
                    
                    numerodec=file.read(2)
                    i=int.from_bytes(numerodec,byteorder='little')
                    self.canvas.create_text(30,260, text='Number of Heads: '+str(i),anchor=tk.W)
                    
                    sectoreso=file.read(4)
                    i=int.from_bytes(sectoreso,byteorder='little')
                    self.canvas.create_text(30,280, text='Hidden Sectors: '+str(i),anchor=tk.W)
                    
                    sectorestl=file.read(4)
                    i=int.from_bytes(sectorestl,byteorder='little')
                    self.canvas.create_text(30,300, text='Large number of sectors: '+str(i),anchor=tk.W, font= bold_font)
                    
                    drive=file.read(1)
                    i=int.from_bytes(drive,byteorder='little')
                    self.canvas.create_text(30,320, text='Drive Number: '+str(i),anchor=tk.W)
                    
                    reservado=file.read(1)
                    i=int.from_bytes(reservado,byteorder='little')
                    self.canvas.create_text(30,340, text='Reserved: '+str(i),anchor=tk.W, font= bold_font)
                    
                    signaturae=file.read(1)
                    i=int.from_bytes(signaturae,byteorder='little')
                    self.canvas.create_text(30,360, text='Extended Boot Signature: '+str(i),anchor=tk.W)
                    
                    volumensn=file.read(4)
                    i=int.from_bytes(volumensn,byteorder='little')
                    self.canvas.create_text(30,380, text='Volume Serial Number: '+str(i),anchor=tk.W)
                    
                    volumen=file.read(11)
                    texto=volumen.decode('utf-8')
                    self.canvas.create_text(30,400, text='Volume Label: '+texto,anchor=tk.W)
                    
                    volumen=file.read(8)
                    texto=volumen.decode('utf-8')
                    self.canvas.create_text(30,420, text='File System Type: '+texto,anchor=tk.W, font= bold_font)
                    
                    file.seek(448,1)
                    signaturabs=file.read(4)
                    i=int.from_bytes(signaturabs,byteorder='little')
                    self.canvas.create_text(30,440, text='Boot Sector signature: '+format(i,'X'),anchor=tk.W)
                else: 
                    self.show_error2
                    
    def determine_type(self, file): 
        ''' determinando el numero de clusters en el sistema de archivos 

            Recuerde que hay 8 valores reservados para el final del archivo
            , 1 para los clústeres defectuosos y las entradas no utilizadas en las ranuras 0 y 1.
        '''
        #primero se verifica si contiene algun FAT 
        file.seek(54)
        i=file.read(8)
        filetype = "".join( chr(x) for x in bytearray(i))
        #print('.',filetype,'.')        
        if 'FAT' not in filetype:
            return '-1' 

        file.seek(32) 
        i=file.read(4)
        large_num_sectors = int.from_bytes(i,byteorder='little')

        file.seek(14)
        i=file.read(2)
        reserved_sectors = int.from_bytes(i,byteorder='little')
        
        file.seek(16)
        i=file.read(1)
        n_fat_copies = int.from_bytes(i,byteorder='little')

        file.seek(22)
        i=file.read(2)
        sectors_por_fat = int.from_bytes(i,byteorder='little')

        sectors_data_area = large_num_sectors - reserved_sectors - (n_fat_copies * sectors_por_fat)

        file.seek(13)
        i=file.read(1)
        sectors_por_cluster = int.from_bytes(i,byteorder='little')

        num_clusters = sectors_data_area / sectors_por_cluster 
        if num_clusters < 4085:
            return '12'
        elif num_clusters >= 4085 and num_clusters < 65525: 
            return '16' 
        elif num_clusters >= 65525:
            return '32'
    
    # para mostrar los clusters correspondientes a un archivo
    def show_clusters_of_a_file(self, first_cluster): 
        with open(self.fname,mode='rb') as file:              
            #se determina el fat
            fat_type = self.determine_type(file)
            
            file.seek(22)
            i=file.read(2)
            sectors_por_fat = int.from_bytes(i,byteorder='little')
            
            file.seek(11)
            i=file.read(2)
            bytes_por_sector = int.from_bytes(i,byteorder='little')

            bytes_por_fat = sectors_por_fat * bytes_por_sector

            if fat_type == '32': 
                size_entry = int('28')
            else: 
                size_entry = int(fat_type) 

            n_entries = (bytes_por_fat * 8) / size_entry

            # Para cualquier FAT , saltando el area reservada 
            file.seek(14) 
            reserved_sectors = file.read(2) ; n_reserved_sectors = int.from_bytes(reserved_sectors, byteorder= 'little')
            file.seek(11)
            bytes_per_sector = file.read(2) ; n_bytes_per_sector = int.from_bytes(bytes_per_sector, byteorder='little')
            file.seek(n_reserved_sectors * n_bytes_per_sector)                
            
            #print(int(n_entries))

            fat = []
            for i in range(int(n_entries)):
                byte=file.read(2)
                i=int.from_bytes(byte,byteorder='little',signed=True)
                fat.append(i) 
            
            print(first_cluster , " -> ", end=" ")
            next_cluster = fat[first_cluster] 
            while next_cluster != -1: 
                print(next_cluster, " -> ",end= " ")
                next_cluster = fat[next_cluster]
                
    def show_fat(self):
        
        if self.fname == None:
            self.show_error1()
        else:
            self.canvas.delete("all")
            w = self.canvas.winfo_width() 
            h = self.canvas.winfo_height()
            self.canvas.delete(tk.ALL)
            with open(self.fname,mode='rb') as file:              
                #se determina el fat
                fat_type = self.determine_type(file)
                
                file.seek(22)
                i=file.read(2)
                sectors_por_fat = int.from_bytes(i,byteorder='little')
                
                file.seek(11)
                i=file.read(2)
                bytes_por_sector = int.from_bytes(i,byteorder='little')

                bytes_por_fat = sectors_por_fat * bytes_por_sector

                if fat_type == '32': 
                    size_entry = int('28')
                else: 
                    size_entry = int(fat_type) 

                n_entries = (bytes_por_fat * 8) / size_entry

                # Para cualquier FAT , saltando el area reservada 
                file.seek(14) 
                reserved_sectors = file.read(2) ; n_reserved_sectors = int.from_bytes(reserved_sectors, byteorder= 'little')
                file.seek(11)
                bytes_per_sector = file.read(2) ; n_bytes_per_sector = int.from_bytes(bytes_per_sector, byteorder='little')
                file.seek(n_reserved_sectors * n_bytes_per_sector)                
                
                #print(int(n_entries))

                fat = []
                for i in range(int(n_entries)):
                    byte=file.read(2)
                    i=int.from_bytes(byte,byteorder='little',signed=True)
                    fat.append(i)            
                cont = 0
                for j in range(0,h,30):
                    for i in range(0,w,30):
                        if fat[cont] != 0:
                            if cont == 0 or cont == 1: 
                                self.canvas.create_rectangle(i,j,i+30,j+30,fill="red", outline = 'black')
                            else:
                                self.canvas.create_rectangle(i,j,i+30,j+30,fill="yellow", outline = 'black')
                                self.canvas.create_text(i+15,j+15,text=str(fat[cont]))
                        else: 
                            self.canvas.create_rectangle(i,j,i+30,j+30,fill="white", outline = 'black') 
                            #self.canvas.create_text(i+15,j+15,text=str(fat[cont]))
                        cont += 1
                        if cont == int(n_entries):
                            return
                    
    def show_rootdir(self):
        with open(self.fname,mode='rb') as file:
            #este campo indica el número de entradas de directorio de 32 bytes en el directorio raíz
            file.seek(17)
            byte = file.read(2) ; n_entr_rootd = int.from_bytes(byte, byteorder='little')

            # nos movemos hasta el root directory 
            file.seek(14)
            i=file.read(2)
            reserved_sectors = int.from_bytes(i,byteorder='little')
            
            file.seek(16)
            i=file.read(1)
            n_fat_copies = int.from_bytes(i,byteorder='little')

            file.seek(22)
            i=file.read(2)
            sectors_por_fat = int.from_bytes(i,byteorder='little')
            
            file.seek(11)
            bytes_per_sector = file.read(2) ; n_bytes_per_sector = int.from_bytes(bytes_per_sector, byteorder='little')

            offset = reserved_sectors + (sectors_por_fat * n_fat_copies)
            offset  = offset * n_bytes_per_sector

            file.seek(offset)
            self.canvas.delete("all")
            fila_in = 20
            for _ in range(n_entr_rootd):                 
                byte = file.read(8)
                filename = "".join( chr(x) for x in bytearray(byte))
                byte = file.read(18) # otros atributos
                byte = file.read(2)
                n_cluster = int.from_bytes(byte,byteorder='little')
                byte = file.read(4)
                file_size = int.from_bytes(byte,byteorder='little')
                if n_cluster != 0:
                    self.canvas.create_text(30,fila_in, text='nombre del archivo: ' + filename,anchor=tk.W) ; fila_in = fila_in + 20
                    self.canvas.create_text(30,fila_in, text='tamaño del archivo: ' + str(file_size),anchor=tk.W) ; fila_in = fila_in + 20
                    self.canvas.create_text(30,fila_in, text='numero del primer cluster: ' + format(n_cluster,'X'),anchor=tk.W) ; fila_in = fila_in + 20
                    fila_in = fila_in + 10

        self.show_clusters_of_a_file(6)

    def show_error1(self):
        msg = "No se ha elegido la imagen del sistema de archivos"
        mb.showerror("Error", msg)
        
    def show_error2(self):
        msg = "El archivo no contiene imagen de sistema de archivos FAT16"
        mb.showerror("Error", msg)
        
if __name__ == "__main__":
    app = App()
    app.mainloop()