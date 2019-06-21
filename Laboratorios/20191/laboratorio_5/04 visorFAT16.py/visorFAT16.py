import tkinter as tk
import tkinter.filedialog as fd
import tkinter.messagebox as mb
from tkinter import font as tkfont

class App(tk.Tk):
    
    
    def __init__(self):
        super().__init__()
        self.fname = None
        self.title("Visor File System FAT16")         
        self.canvas = tk.Canvas(self, width=500, height=600, bg="white")         
        btn_file = tk.Button(self, text="Choose file system image",command=self.choose_file)         
        btn_showBoot = tk.Button(self, text="Boot Sector",command=self.show_bootsectorFAT)
        btn_showFat = tk.Button(self, text="File Allocation Table",command=self.show_fat)
        btn_showRootDir = tk.Button(self, text="Root directory",command=self.show_rootdir)                  
        btn_file.grid(row=0, column=0, columnspan=2, padx=60,pady=20 )
                
        btn_showBoot.grid(row=2, column=0, padx=60,pady=20)
        btn_showFat.grid(row=2, column=1, padx=60,pady=20)
        btn_showRootDir.grid(row=3, column=1, padx=60,pady=20)
        self.canvas.grid(row=4, column=0,rowspan=3, columnspan=2)
        
        
    def choose_file(self):
        filetypes = (("File sistem images", "*.img"),("All files", "*"))
        filename = fd.askopenfilename(title="Open file", initialdir="/home/alejandro/SourceCode/Python", filetypes=filetypes)
        self.fname = filename
        
    def show_bootsectorFAT(self):
        if self.fname == None:
            self.show_error1()
        else:    
            with open(self.fname,mode='rb') as file:
                self.canvas.delete("all")
                bold_font = tkfont.Font(family="Helvetica", size=12, weight="bold")

                byte_1 = file.read(1) ; num_1 = int.from_bytes(byte_1, byteorder='little')
                byte_2 = file.read(1) ; num_2 = int.from_bytes(byte_2, byteorder='little')
                byte_3 = file.read(1) ; num_3 = int.from_bytes(byte_3, byteorder='little')

                self.canvas.create_text(20,20,text='Code to jump the bootstrap  code: ' +
                                        ((str(hex(num_1))).upper())[2:] + ' ' + 
                                        ((str(hex(num_2))).upper())[2:] + ' ' +
                                        ((str(hex(num_3))).upper())[2:] ,anchor=tk.W)

                oem_name_b = file.read(8) ; oem_name = "".join( chr(x) for x in bytearray(oem_name_b))
                self.canvas.create_text(20,40, text='Oem ID - Name of the formatting OS: ' + oem_name, anchor=tk.W)

                bytes_per_sector = file.read(2) ; n_bytes_per_sector = int.from_bytes(bytes_per_sector, byteorder='little')  
                self.canvas.create_text(20,60, text='Bytes por sector: ' + str(n_bytes_per_sector), anchor= tk.W, font= bold_font)
                
                sectors_por_cluster = file.read(1) ; n_sectors_por_cluster =  int.from_bytes(sectors_por_cluster, byteorder='little')
                self.canvas.create_text(20,80, text='Sectors por cluster: ' + str(n_sectors_por_cluster), anchor= tk.W, font= bold_font)

                #muestra el numero de sectores en el area reservada
                reserved_sectors = file.read(2) ; n_reserved_sectors = int.from_bytes(reserved_sectors, byteorder= 'little')
                self.canvas.create_text(20,100, text='Reserved sectors: ' + str(n_reserved_sectors), anchor= tk.W, font= bold_font)

                fat_copies = file.read(1) ; n_fat_copies = int.from_bytes(fat_copies, byteorder= 'little')
                self.canvas.create_text(20,120, text='Number of FAT copies: ' + str(n_fat_copies), anchor= tk.W)
                
                # muestra el numero de entradas de directorio de 32 bytes en el directorio raiz
                root_poss_entries = file.read(2) ; n_root_poss_entries = int.from_bytes(root_poss_entries, byteorder= 'little')
                self.canvas.create_text(20,140, text='Number of possible root entries: ' + str(n_root_poss_entries), anchor= tk.W)

                b_number_of_sectors = file.read(2) ; number_of_sectors = int.from_bytes(b_number_of_sectors, byteorder= 'little')
                self.canvas.create_text(20,160, text='Small number of sectors: ' + str(number_of_sectors), anchor= tk.W)

                #algo para discos
                b_media_desc = file.read(1) ; media_desc = int.from_bytes(b_media_desc, byteorder='little')
                self.canvas.create_text(20,180, text='Media descriptor: ' + ((str(hex(media_desc))).upper())[2:], anchor= tk.W)

                #numero de sectores ocupados por una FAT 
                b_sect_per_FAT = file.read(2) ; n_sect_per_FAT = int.from_bytes(b_sect_per_FAT, byteorder='little')
                self.canvas.create_text(20,200, text='Sectors per FAT: ' + str(n_sect_per_FAT), anchor= tk.W, font= bold_font)

                #numero de sectores por pista 
                b_sect_per_track = file.read(2) ; n_sect_per_track = int.from_bytes(b_sect_per_track, byteorder='little')
                self.canvas.create_text(20,220, text='Sectors per Track: ' + str(n_sect_per_track), anchor= tk.W)

                #numero de cabezas 
                b_heads = file.read(2) ; n_heads = int.from_bytes(b_heads, byteorder='little')
                self.canvas.create_text(20,240, text='Number of Heads: ' + str(n_heads), anchor= tk.W)

                #Número de sectores físicos ocultos que preceden al volumen FAT.
                b_hidd_sectors = file.read(4) ; n_hidd_sectors = int.from_bytes(b_hidd_sectors, byteorder='little')
                self.canvas.create_text(20,260, text='Hidden Sectors: ' + str(n_hidd_sectors), anchor= tk.W)

                #Numero total de sectores en el volumen FAT 
                b_num_sectors = file.read(4) ; num_sectors = int.from_bytes(b_num_sectors, byteorder='little')
                self.canvas.create_text(20,280, text='Large number of sectors: ' + str(num_sectors), anchor= tk.W, font= bold_font)

                '''
                Dado que los siguientes campos cambian dependiendo de si el volumen es FAT12 / 16 o FAT32, 
                se debe determinar el tipo de FAT antes de referir estos campos. 
                También hay algunos campos que existen solo en volúmenes FAT32 y no existen en volúmenes FAT12 / 16.
                Este codigo cumple para FAT 12/16
                '''
                b_drive_num = file.read(1) ; drive_num = int.from_bytes(b_drive_num, byteorder='little')
                self.canvas.create_text(20,300, text='Drive Number: ' + str(drive_num), anchor= tk.W)

                b_reserved = file.read(1) ; n_reserved = int.from_bytes(b_reserved, byteorder='little')
                self.canvas.create_text(20,320, text='Reserved: ' + str(n_reserved), anchor= tk.W)

                b_boot_sig = file.read(1) ; n_boot_sig = int.from_bytes(b_boot_sig, byteorder='little')
                self.canvas.create_text(20,340, text='Extended Boot Signature: ' + str(n_boot_sig), anchor= tk.W)
                
                #Número de serie del volumen utilizado con BS_VolLab para rastrear un volumen en el almacenamiento extraíble.                
                b_vol_id = file.read(4) ; n_vol_id = int.from_bytes(b_vol_id, byteorder='little')
                self.canvas.create_text(20,360, text='Volumen Serial Number: ' + str(n_vol_id), anchor= tk.W)

                b_vol_lab = file.read(11) ; vol_lab = "".join( chr(x) for x in bytearray(b_vol_lab))                
                self.canvas.create_text(20,380, text='Volumen Label: ' +  vol_lab, anchor= tk.W)

                b_fs_type = file.read(8) ; fs_type = "".join( chr(x) for x in bytearray(b_fs_type))
                self.canvas.create_text(20,400, text='File System Type: ' + fs_type, anchor= tk.W, font= bold_font)
                
                #Bootstrap program
                file.seek(448,1)

                b_bs_sign = file.read(2) ; bs_sign = int.from_bytes(b_bs_sign, byteorder='little')
                self.canvas.create_text(20,420, text='Boot Sector Signature: ' + ((str(hex(bs_sign))).upper())[2:], anchor= tk.W, font= bold_font)

    def show_bootsectorExt2(self):
        if self.fname == None:
            self.show_error1()
        else:    
            with open(self.fname,mode='rb') as file:
                self.canvas.delete("all")
                file.seek(1024) #siempre se encuentra aca 

                inodes=file.read(4)
                i=int.from_bytes(inodes,byteorder='little')
                self.canvas.create_text(20,20, text="Numero de inodos en el sistema de archivos: "+str(i),anchor=tk.W)
                
                blocks=file.read(4)
                i=int.from_bytes(blocks,byteorder='little')
                self.canvas.create_text(20,40, text='Numero de bloques en el sistema de archivos: '+str(i),anchor=tk.W)
                
                reserved_blocks=file.read(4)
                i=int.from_bytes(reserved_blocks,byteorder='little')
                self.canvas.create_text(20,60, text='Numero de bloques reservados en el sistema de archivos: '+str(i),anchor=tk.W)
                
                free_blocks=file.read(4)
                i=int.from_bytes(free_blocks,byteorder='little')
                self.canvas.create_text(20,80, text='Numero de bloques libres: '+str(i),anchor=tk.W)
                
                free_inodes=file.read(4)
                i=int.from_bytes(free_inodes,byteorder='little')
                self.canvas.create_text(20,100, text='Numero de inodos libres: '+str(i),anchor=tk.W)
                
                first_data_block = file.read(4)
                i=int.from_bytes(first_data_block,byteorder='little')
                self.canvas.create_text(20,120, text='Bloque donde inicia grupo 0: '+str(i),anchor=tk.W)
                
                log_block_size=file.read(4)
                i=int.from_bytes(log_block_size,byteorder='little')                  
                self.canvas.create_text(20,140, text='Tamano de bloque: '+str(i),anchor=tk.W)
                
                file.seek(4,1)
                blocks_per_group=file.read(4)
                i=int.from_bytes(blocks_per_group,byteorder='little')
                self.canvas.create_text(20,160, text='Numero de bloques en cada grupo: '+str(i),anchor=tk.W)
                
                file.seek(4,1)
                inodes_per_group=file.read(4)
                i=int.from_bytes(inodes_per_group,byteorder='little')
                self.canvas.create_text(20,180, text='Numero de inodos en cada grupo: '+str(i),anchor=tk.W)
                
                file.seek(12,1)
                magic=file.read(2)
                i=int.from_bytes(magic,byteorder='little')
                self.canvas.create_text(20,200, text='Signatura: '+str(hex(i)),anchor=tk.W)
                
                file.seek(26,1)
                first_ino = file.read(4)
                i=int.from_bytes(first_ino,byteorder='little')
                self.canvas.create_text(20,220, text='Primer inodo no reservado en el sistema de archivo: '+str(i),anchor=tk.W)
                
                ino_size=file.read(2)
                i=int.from_bytes(ino_size,byteorder='little')
                self.canvas.create_text(20,240, text='Tamano del inodo: '+str(i),anchor=tk.W)
                
                file.seek(30,1)
                name = file.read(16)
                n=name.decode()
                self.canvas.create_text(20,260, text='Nombre del volúmen: '+n,anchor=tk.W)
        
    def show_fat(self):
        if self.fname == None:
            self.show_error1()
        else:
            self.canvas.delete("all")
            w = self.canvas.winfo_width() 
            h = self.canvas.winfo_height()
            self.canvas.delete(tk.ALL)
            with open(self.fname,mode='rb') as file:              
                # Para cualquier FAT 
                file.seek(14) 
                reserved_sectors = file.read(2) ; n_reserved_sectors = int.from_bytes(reserved_sectors, byteorder= 'little')
                file.seek(11)
                bytes_per_sector = file.read(2) ; n_bytes_per_sector = int.from_bytes(bytes_per_sector, byteorder='little')
                print(reserved_sectors * bytes_per_sector)
                file.seek(reserved_sectors * bytes_per_sector)
                
                ## Lee solo las primeras 16 entradas de la FAT
                fat = []
                for i in range(512):
                    byte=file.read(2)
                    i=int.from_bytes(byte,byteorder='little')
                    fat.append(i)            
                cont = 0
                for j in range(0,h,20):
                    for i in range(0,w,20):
                        if fat[cont] != 0:
                            self.canvas.create_rectangle(i,j,i+20,j+20,fill="green", outline = 'black')
                        else: 
                            self.canvas.create_rectangle(i,j,i+20,j+20,fill="white", outline = 'black') 
                        cont += 1
                        if cont == 512:
                            return
                        
    def show_rootdir(self):
        pass
                       
    def show_error1(self):
        msg = "No se ha elegido la imagen del sistema de archivos"
        mb.showerror("Error", msg)
            
    def show_error2(self):
        msg = "El archivo no contiene imagen de sistema de archivos Ext2"
        mb.showerror("Error", msg)
            
if __name__ == "__main__":
       app = App()
       app.mainloop()