--
-- PostgreSQL database dump
--

\restrict bCg1WszSMWglXln1fKKbFCSQMfW6dyGPjYnlM79t3W9SSdRbe8BRGjUQhIvCOsD

-- Dumped from database version 16.13
-- Dumped by pg_dump version 16.13

-- Started on 2026-05-01 20:44:27

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- TOC entry 216 (class 1259 OID 16455)
-- Name: alumnos; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.alumnos (
    id integer NOT NULL,
    nombres character varying(100),
    apellidos character varying(100),
    carnet character varying(20)
);


ALTER TABLE public.alumnos OWNER TO postgres;

--
-- TOC entry 222 (class 1259 OID 16476)
-- Name: alumnos_cursos; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.alumnos_cursos (
    id integer NOT NULL,
    alumno_id integer,
    curso_id integer
);


ALTER TABLE public.alumnos_cursos OWNER TO postgres;

--
-- TOC entry 221 (class 1259 OID 16475)
-- Name: alumnos_cursos_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.alumnos_cursos_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.alumnos_cursos_id_seq OWNER TO postgres;

--
-- TOC entry 4922 (class 0 OID 0)
-- Dependencies: 221
-- Name: alumnos_cursos_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.alumnos_cursos_id_seq OWNED BY public.alumnos_cursos.id;


--
-- TOC entry 215 (class 1259 OID 16454)
-- Name: alumnos_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.alumnos_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.alumnos_id_seq OWNER TO postgres;

--
-- TOC entry 4923 (class 0 OID 0)
-- Dependencies: 215
-- Name: alumnos_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.alumnos_id_seq OWNED BY public.alumnos.id;


--
-- TOC entry 224 (class 1259 OID 16493)
-- Name: alumnos_seccion; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.alumnos_seccion (
    id integer NOT NULL,
    alumno_id integer,
    seccion_id integer
);


ALTER TABLE public.alumnos_seccion OWNER TO postgres;

--
-- TOC entry 223 (class 1259 OID 16492)
-- Name: alumnos_seccion_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.alumnos_seccion_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.alumnos_seccion_id_seq OWNER TO postgres;

--
-- TOC entry 4924 (class 0 OID 0)
-- Dependencies: 223
-- Name: alumnos_seccion_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.alumnos_seccion_id_seq OWNED BY public.alumnos_seccion.id;


--
-- TOC entry 218 (class 1259 OID 16462)
-- Name: cursos; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.cursos (
    id integer NOT NULL,
    nombre character varying(100),
    codigo character varying(20)
);


ALTER TABLE public.cursos OWNER TO postgres;

--
-- TOC entry 217 (class 1259 OID 16461)
-- Name: cursos_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.cursos_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.cursos_id_seq OWNER TO postgres;

--
-- TOC entry 4925 (class 0 OID 0)
-- Dependencies: 217
-- Name: cursos_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.cursos_id_seq OWNED BY public.cursos.id;


--
-- TOC entry 220 (class 1259 OID 16469)
-- Name: secciones; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.secciones (
    id integer NOT NULL,
    nombre character varying(50),
    jornada character varying(50)
);


ALTER TABLE public.secciones OWNER TO postgres;

--
-- TOC entry 219 (class 1259 OID 16468)
-- Name: secciones_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.secciones_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.secciones_id_seq OWNER TO postgres;

--
-- TOC entry 4926 (class 0 OID 0)
-- Dependencies: 219
-- Name: secciones_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.secciones_id_seq OWNED BY public.secciones.id;


--
-- TOC entry 4755 (class 2604 OID 16458)
-- Name: alumnos id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.alumnos ALTER COLUMN id SET DEFAULT nextval('public.alumnos_id_seq'::regclass);


--
-- TOC entry 4758 (class 2604 OID 16479)
-- Name: alumnos_cursos id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.alumnos_cursos ALTER COLUMN id SET DEFAULT nextval('public.alumnos_cursos_id_seq'::regclass);


--
-- TOC entry 4759 (class 2604 OID 16496)
-- Name: alumnos_seccion id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.alumnos_seccion ALTER COLUMN id SET DEFAULT nextval('public.alumnos_seccion_id_seq'::regclass);


--
-- TOC entry 4756 (class 2604 OID 16465)
-- Name: cursos id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.cursos ALTER COLUMN id SET DEFAULT nextval('public.cursos_id_seq'::regclass);


--
-- TOC entry 4757 (class 2604 OID 16472)
-- Name: secciones id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.secciones ALTER COLUMN id SET DEFAULT nextval('public.secciones_id_seq'::regclass);


--
-- TOC entry 4767 (class 2606 OID 16481)
-- Name: alumnos_cursos alumnos_cursos_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.alumnos_cursos
    ADD CONSTRAINT alumnos_cursos_pkey PRIMARY KEY (id);


--
-- TOC entry 4761 (class 2606 OID 16460)
-- Name: alumnos alumnos_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.alumnos
    ADD CONSTRAINT alumnos_pkey PRIMARY KEY (id);


--
-- TOC entry 4769 (class 2606 OID 16498)
-- Name: alumnos_seccion alumnos_seccion_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.alumnos_seccion
    ADD CONSTRAINT alumnos_seccion_pkey PRIMARY KEY (id);


--
-- TOC entry 4763 (class 2606 OID 16467)
-- Name: cursos cursos_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.cursos
    ADD CONSTRAINT cursos_pkey PRIMARY KEY (id);


--
-- TOC entry 4765 (class 2606 OID 16474)
-- Name: secciones secciones_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.secciones
    ADD CONSTRAINT secciones_pkey PRIMARY KEY (id);


--
-- TOC entry 4770 (class 2606 OID 16482)
-- Name: alumnos_cursos alumnos_cursos_alumno_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.alumnos_cursos
    ADD CONSTRAINT alumnos_cursos_alumno_id_fkey FOREIGN KEY (alumno_id) REFERENCES public.alumnos(id);


--
-- TOC entry 4771 (class 2606 OID 16487)
-- Name: alumnos_cursos alumnos_cursos_curso_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.alumnos_cursos
    ADD CONSTRAINT alumnos_cursos_curso_id_fkey FOREIGN KEY (curso_id) REFERENCES public.cursos(id);


--
-- TOC entry 4772 (class 2606 OID 16499)
-- Name: alumnos_seccion alumnos_seccion_alumno_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.alumnos_seccion
    ADD CONSTRAINT alumnos_seccion_alumno_id_fkey FOREIGN KEY (alumno_id) REFERENCES public.alumnos(id);


--
-- TOC entry 4773 (class 2606 OID 16504)
-- Name: alumnos_seccion alumnos_seccion_seccion_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.alumnos_seccion
    ADD CONSTRAINT alumnos_seccion_seccion_id_fkey FOREIGN KEY (seccion_id) REFERENCES public.secciones(id);


-- Completed on 2026-05-01 20:44:28

--
-- PostgreSQL database dump complete
--

\unrestrict bCg1WszSMWglXln1fKKbFCSQMfW6dyGPjYnlM79t3W9SSdRbe8BRGjUQhIvCOsD

