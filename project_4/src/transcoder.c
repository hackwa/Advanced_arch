#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#define	BIAS		(0x84)		/* Bias for linear code. */
#define CLIP            8159

#define	SIGN_BIT	(0x80)		/* Sign bit for a A-law byte. */
#define	QUANT_MASK	(0xf)		/* Quantization field mask. */
#define	NSEGS		(8)		/* Number of A-law segments. */
#define	SEG_SHIFT	(4)		/* Left shift for segment number. */
#define	SEG_MASK	(0x70)	

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
    
    
    short int extra_thing;
    char some_fact[4];
    char again_something[8];
    
    char subchunk2_id[4];
    int subchunk2_size;			
} header;

typedef struct header_file* header_p;

static short seg_aend[8] = {0x1F, 0x3F, 0x7F, 0xFF,
			    0x1FF, 0x3FF, 0x7FF, 0xFFF};
static short seg_uend[8] = {0x3F, 0x7F, 0xFF, 0x1FF,
			    0x3FF, 0x7FF, 0xFFF, 0x1FFF};
search(
	short		val,
	short		*table,
	short		size)
{
	short		i;

	for (i = 0; i < size; i++) {
		if (val <= *table++)
			return (i);
	}
	return (size);
}
unsigned char
Snack_Lin2Alaw(
	short		pcm_val)	/* 2's complement (16-bit range) */
{
	short		mask;
	short		seg;
	unsigned char	aval;

	pcm_val = pcm_val >> 3;

	if (pcm_val >= 0) {
		mask = 0xD5;		/* sign (7th) bit = 1 */
	} else {
		mask = 0x55;		/* sign bit = 0 */
		pcm_val = -pcm_val - 1;
	}

	/* Convert the scaled magnitude to segment number. */
	seg = search(pcm_val, seg_aend, 8);

	/* Combine the sign, segment, and quantization bits. */

	if (seg >= 8)		/* out of range, return maximum value. */
		return (unsigned char) (0x7F ^ mask);
	else {
		aval = (unsigned char) seg << SEG_SHIFT;
		if (seg < 2)
			aval |= (pcm_val >> 1) & QUANT_MASK;
		else
			aval |= (pcm_val >> seg) & QUANT_MASK;
		return (aval ^ mask);
	}
}
unsigned char
Snack_Lin2Mulaw(
	short		pcm_val)	/* 2's complement (16-bit range) */
{
	short		mask;
	short		seg;
	unsigned char	uval;

	/* Get the sign and the magnitude of the value. */
	pcm_val = pcm_val >> 2;
	if (pcm_val < 0) {
		pcm_val = -pcm_val;
		mask = 0x7F;
	} else {
		mask = 0xFF;
	}
        if ( pcm_val > CLIP ) pcm_val = CLIP;		/* clip the magnitude */
	pcm_val += (BIAS >> 2);

	/* Convert the scaled magnitude to segment number. */
	seg = search(pcm_val, seg_uend, 8);

	/*
	 * Combine the sign, segment, quantization bits;
	 * and complement the code word.
	 */
	if (seg >= 8)		/* out of range, return maximum value. */
		return (unsigned char) (0x7F ^ mask);
	else {
		uval = (unsigned char) (seg << 4) | ((pcm_val >> (seg + 1)) & 0xF);
		return (uval ^ mask);
	}

}

int main(){
    uint16_t pcm_val;
    
    char meta2[44];
    FILE *fp,*fo,*fso,*falaw,*foa;
    
    header_p meta = (header_p)malloc(sizeof(header));
    
       
    char *filename = "A_eng_f8.wav";
    char *sofilename = "Au8A_eng_f8.wav";
    char *alawfilename = "1449183713-A_eng_f8.wav";
    
    
    fp = fopen(filename,"rb");
    fo = fopen("output_u.wav","wb");
    fso = fopen(sofilename,"rb");
    falaw = fopen(alawfilename,"rb");
    foa = fopen("output_a.wav","wb");
	fread(meta, 1, sizeof(header), fso);
        fwrite(meta,1, sizeof(header), fo);
        fclose(fso);
        fread(meta,1,sizeof(header),falaw);
        fwrite(meta,1,sizeof(header),foa);
        fclose(falaw);
        
        fread(meta2,44,1,fp); 
     int count = 0;   
    while( fread(&pcm_val,(sizeof(pcm_val)),1,fp) >= 1 )
    {

   int8_t ch =  Snack_Lin2Mulaw(pcm_val); 
   int8_t ch_a = Snack_Lin2Alaw(pcm_val);
   
    if(count%2 == 0){
    fwrite(&ch,sizeof(ch),1,fo);
    fwrite(&ch_a,sizeof(ch_a),1,foa);
   }
   count++;
    
    
    }
    int a;
    
    fclose(fp);
    fclose(fo);
    
    free(meta);
    scanf("%d",&a);
    return 0;
}