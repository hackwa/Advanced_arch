#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define	BIAS		(0x84)		/* Bias for linear code. */
#define CLIP            8159
#define	SIGN_BIT	(0x80)		/* Sign bit for a A-law byte. */
#define	QUANT_MASK	(0xf)		/* Quantization field mask. */
#define	NSEGS		(8)		/* Number of A-law segments. */
#define	SEG_SHIFT	(4)		/* Left shift for segment number. */
#define	SEG_MASK	(0x70)		/* Segment field mask. */
typedef struct header_file
{
    char chunk_id[4];
    int chunk_size;
    char format[4];
    char subchunk1_id[4];
    int subchunk1_size;
    short int audio_format;
    short int num_channels;
    int sample_rate;			// sample_rate denotes the sampling rate.
    int byte_rate;
    short int block_align;
    short int bits_per_sample;
    char subchunk2_id[4];
    int subchunk2_size;			// subchunk2_size denotes the number of samples.
} header;

typedef struct header_file* header_p;
short
Snack_Mulaw2Lin(
	unsigned char	u_val)
{
	short		t;

	/* Complement to obtain normal u-law value. */
	u_val = ~u_val;

	/*
	 * Extract and bias the quantization bits. Then
	 * shift up by the segment number and subtract out the bias.
	 */
	t = ((u_val & QUANT_MASK) << 3) + BIAS;
	t <<= ((unsigned)u_val & SEG_MASK) >> SEG_SHIFT;

	return ((u_val & SIGN_BIT) ? (BIAS - t) : (t - BIAS));
}
short
Snack_Alaw2Lin(
	unsigned char	a_val)
{
	short		t;
	short		seg;

	a_val ^= 0x55;

	t = (a_val & QUANT_MASK) << 4;
	seg = ((unsigned)a_val & SEG_MASK) >> SEG_SHIFT;
	switch (seg) {
	case 0:
		t += 8;
		break;
	case 1:
		t += 0x108;
		break;
	default:
		t += 0x108;
		t <<= seg - 1;
	}
	return ((a_val & SIGN_BIT) ? t : -t);
}

int main(){
    header_p meta = (header_p)malloc(sizeof(header));
        
    char *filename = "A_eng_f8.wav";
    char *refilename = "Au8A_eng_f8.wav";
    uint8_t pcm_val;
    unsigned char ch;
    char meta2[57];
    FILE *fp,*fo,*fre,*foa,*falaw;
    
    fp = fopen(filename,"rb");
    fo = fopen("output_decoder_u.wav","wb");
    foa = fopen("oputput_decoder_a.wav","wb");
    fre = fopen(refilename,"rb");
    falaw = fopen("1449183713-A_eng_f8.wav","rb");
	fread(meta, 1, sizeof(header), fp);
        fwrite(meta,1, sizeof(header), fo);
        fwrite(meta,1,sizeof(header),foa);
        fclose(fp);
        
        fread(meta2,57,1,fre); 
        fread(meta2,57,1,falaw);
        
      
    while( fread(&pcm_val,(sizeof(pcm_val)),1,fre) >= 1 )
    {
    
  
   int16_t ch = Snack_Mulaw2Lin(pcm_val); 
   
   fwrite(&ch,sizeof(ch),1,fo);
   fwrite(&ch,sizeof(ch),1,fo);
    }
    while (fread(&pcm_val,(sizeof(pcm_val)),1,falaw) >= 1)
    {
        int16_t ch2 = Snack_Alaw2Lin(pcm_val);
        fwrite(&ch2,sizeof(ch2),1,foa);
        fwrite(&ch2,sizeof(ch2),1,foa);
    }
    int a;
    
    fclose(fre);
    fclose(fo);
    
    free(meta);
    scanf("%d",&a);
    return 0;
}
